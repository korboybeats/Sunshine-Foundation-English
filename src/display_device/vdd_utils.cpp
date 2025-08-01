#define WIN32_LEAN_AND_MEAN

#include "vdd_utils.h"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <filesystem>
#include <sstream>
#include <thread>

#include "src/confighttp.h"
#include "src/globals.h"
#include "src/platform/common.h"
#include "src/rtsp.h"
#include "src/system_tray.h"
#include "to_string.h"

namespace display_device {
  namespace vdd_utils {

    const wchar_t *kVddPipeName = L"\\\\.\\pipe\\ZakoVDDPipe";
    const DWORD kPipeTimeoutMs = 5000;
    const DWORD kPipeBufferSize = 4096;
    const std::chrono::milliseconds kDefaultDebounceInterval { 2000 };

    // 上次切换显示器的时间点
    static std::chrono::steady_clock::time_point last_toggle_time { std::chrono::steady_clock::now() };
    // 防抖间隔
    static std::chrono::milliseconds debounce_interval { kDefaultDebounceInterval };

    std::chrono::milliseconds
    calculate_exponential_backoff(int attempt) {
      auto delay = kInitialRetryDelay * (1 << attempt);
      return std::min(delay, kMaxRetryDelay);
    }

    bool
    execute_vdd_command(const std::string &action) {
      static const std::string kDevManPath = (std::filesystem::path(SUNSHINE_ASSETS_DIR).parent_path() / "tools" / "DevManView.exe").string();
      static const std::string kDriverName = "Zako Display Adapter";

      boost::process::environment _env = boost::this_process::environment();
      auto working_dir = boost::filesystem::path();
      std::error_code ec;

      std::string cmd = kDevManPath + " /" + action + " \"" + kDriverName + "\"";

      for (int attempt = 0; attempt < kMaxRetryCount; ++attempt) {
        auto child = platf::run_command(true, true, cmd, working_dir, _env, nullptr, ec, nullptr);
        if (!ec) {
          BOOST_LOG(info) << "Successfully executed VDD " << action << " command";
          child.detach();
          return true;
        }

        auto delay = calculate_exponential_backoff(attempt);
        BOOST_LOG(warning) << "Failed to execute VDD " << action << " command (attempt "
                           << (attempt + 1) << "/" << kMaxRetryCount
                           << "): " << ec.message() << ". Retrying in "
                           << delay.count() << "ms";
        std::this_thread::sleep_for(delay);
      }

      BOOST_LOG(error) << "Failed to execute VDD " << action << " command after maximum retries";
      return false;
    }

    HANDLE
    connect_to_pipe_with_retry(const wchar_t *pipe_name, int max_retries) {
      HANDLE hPipe = INVALID_HANDLE_VALUE;
      int attempt = 0;
      auto retry_delay = kInitialRetryDelay;

      while (attempt < max_retries) {
        hPipe = CreateFileW(
          pipe_name,
          GENERIC_READ | GENERIC_WRITE,
          0,
          NULL,
          OPEN_EXISTING,
          FILE_FLAG_OVERLAPPED,  // 使用异步IO
          NULL);

        if (hPipe != INVALID_HANDLE_VALUE) {
          DWORD mode = PIPE_READMODE_MESSAGE;
          if (SetNamedPipeHandleState(hPipe, &mode, NULL, NULL)) {
            return hPipe;
          }
          CloseHandle(hPipe);
        }

        ++attempt;
        retry_delay = calculate_exponential_backoff(attempt);
        std::this_thread::sleep_for(retry_delay);
      }
      return INVALID_HANDLE_VALUE;
    }

    bool
    execute_pipe_command(const wchar_t *pipe_name, const wchar_t *command, std::string *response) {
      auto hPipe = connect_to_pipe_with_retry(pipe_name);
      if (hPipe == INVALID_HANDLE_VALUE) {
        BOOST_LOG(error) << "Failed to connect to MTT virtual display pipe after multiple retries";
        return false;
      }

      // 异步IO结构体
      OVERLAPPED overlapped = { 0 };
      overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

      struct HandleGuard {
        HANDLE handle;
        ~HandleGuard() {
          if (handle) CloseHandle(handle);
        }
      } event_guard { overlapped.hEvent };

      // 发送命令（使用宽字符版本）
      DWORD bytesWritten;
      size_t cmd_len = (wcslen(command) + 1) * sizeof(wchar_t);  // 包含终止符
      if (!WriteFile(hPipe, command, (DWORD) cmd_len, &bytesWritten, &overlapped)) {
        if (GetLastError() != ERROR_IO_PENDING) {
          BOOST_LOG(error) << L"Sending " << command << L" command failed, error code: " << GetLastError();
          return false;
        }

        // 等待写入完成
        DWORD waitResult = WaitForSingleObject(overlapped.hEvent, kPipeTimeoutMs);
        if (waitResult != WAIT_OBJECT_0) {
          BOOST_LOG(error) << L"Sending " << command << L" command timed out";
          return false;
        }
      }

      // 读取响应
      if (response) {
        char buffer[kPipeBufferSize];
        DWORD bytesRead;
        if (!ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, &overlapped)) {
          if (GetLastError() != ERROR_IO_PENDING) {
            BOOST_LOG(warning) << "Failed to read response, error code: " << GetLastError();
            return false;
          }

          DWORD waitResult = WaitForSingleObject(overlapped.hEvent, kPipeTimeoutMs);
          if (waitResult == WAIT_OBJECT_0 && GetOverlappedResult(hPipe, &overlapped, &bytesRead, FALSE)) {
            buffer[bytesRead] = '\0';
            *response = std::string(buffer, bytesRead);
          }
        }
      }

      return true;
    }

    bool
    reload_driver() {
      std::string response;
      return execute_pipe_command(kVddPipeName, L"RELOAD_DRIVER", &response);
    }

    bool
    create_vdd_monitor() {
      std::string response;
      if (!execute_pipe_command(kVddPipeName, L"CREATEMONITOR", &response)) {
        BOOST_LOG(error) << "Failed to create virtual display";
        return false;
      }
#if defined SUNSHINE_TRAY && SUNSHINE_TRAY >= 1
      system_tray::update_tray_vmonitor_checked(1);
#endif
      BOOST_LOG(info) << "Virtual display creation completed, responding: " << response;
      return true;
    }

    bool
    destroy_vdd_monitor() {
      std::string response;
      if (!execute_pipe_command(kVddPipeName, L"DESTROYMONITOR", &response)) {
        BOOST_LOG(error) << "Failed to remove virtual display";
        return false;
      }
#if defined SUNSHINE_TRAY && SUNSHINE_TRAY >= 1
      system_tray::update_tray_vmonitor_checked(0);
#endif
      BOOST_LOG(info) << "Virtual display removal completed, responding: " << response;
      return true;
    }

    void
    enable_vdd() {
      execute_vdd_command("enable");
    }

    void
    disable_vdd() {
      execute_vdd_command("disable");
    }

    void
    disable_enable_vdd() {
      execute_vdd_command("disable_enable");
    }

    bool
    is_display_on() {
      return !display_device::find_device_by_friendlyname(zako_name).empty();
    }

    void
    toggle_display_power() {
      auto now = std::chrono::steady_clock::now();

      if (now - last_toggle_time < debounce_interval) {
        BOOST_LOG(debug) << "Ignoring rapid repeated display toggle requests, please wait for "
                         << std::chrono::duration_cast<std::chrono::seconds>(
                              debounce_interval - (now - last_toggle_time))
                              .count()
                         << " seconds.";
        return;
      }

      last_toggle_time = now;

      if (!is_display_on()) {
        if (create_vdd_monitor()) {
          std::thread([]() {
            // Windows弹窗确认
            auto future = std::async(std::launch::async, []() {
              return MessageBoxW(nullptr,
                       L"A virtual display has already been created. Would you like to continue using it?\n\n"
                       L"If no action is taken, the display will automatically turn off in 20 seconds.",
                       L"Display confirmation",
                       MB_YESNO | MB_ICONQUESTION) == IDYES;
            });

            // 等待20秒超时
            if (future.wait_for(std::chrono::seconds(20)) != std::future_status::ready || !future.get()) {
              BOOST_LOG(info) << "Virtual display was automatically removed due to no user confirmation or timeout.";
              HWND hwnd = FindWindowW(L"#32770", L"Display confirmation");
              if (hwnd && IsWindow(hwnd)) {
                // 发送退出命令并等待窗口关闭
                PostMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDNO, BN_CLICKED), 0);
                PostMessage(hwnd, WM_CLOSE, 0, 0);

                for (int i = 0; i < 5 && IsWindow(hwnd); ++i) {
                  std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }

                // 如果窗口还存在，尝试强制关闭
                if (IsWindow(hwnd)) {
                  BOOST_LOG(warning) << "The confirmation window could not be closed normally. Attempting to terminate the process.";
                  EndDialog(hwnd, IDNO);
                }
              }
              destroy_vdd_monitor();
            }
            else {
              BOOST_LOG(info) << "User confirms to keep the virtual display active.";
            }
          }).detach();
        }
      }
      else {
        destroy_vdd_monitor();
      }
    }

    VddSettings
    prepare_vdd_settings(const parsed_config_t &config) {
      auto is_res_cached = false;
      auto is_fps_cached = false;
      std::ostringstream res_stream, fps_stream;

      res_stream << '[';
      fps_stream << '[';

      // 检查分辨率是否已缓存
      for (const auto &res : config::nvhttp.resolutions) {
        res_stream << res << ',';
        if (config.resolution && res == to_string(*config.resolution)) {
          is_res_cached = true;
        }
      }

      // 检查帧率是否已缓存
      for (const auto &fps : config::nvhttp.fps) {
        fps_stream << fps << ',';
        if (config.refresh_rate && std::to_string(fps) == to_string(*config.refresh_rate)) {
          is_fps_cached = true;
        }
      }

      // 如果需要更新设置
      bool needs_update = (!is_res_cached || !is_fps_cached) && config.resolution;
      if (needs_update) {
        if (!is_res_cached) {
          res_stream << to_string(*config.resolution);
        }
        if (!is_fps_cached) {
          fps_stream << to_string(*config.refresh_rate);
        }
      }

      // 移除最后的逗号并添加结束括号
      auto res_str = res_stream.str();
      auto fps_str = fps_stream.str();
      if (res_str.back() == ',') res_str.pop_back();
      if (fps_str.back() == ',') fps_str.pop_back();
      res_str += ']';
      fps_str += ']';

      return { res_str, fps_str, needs_update };
    }
  }  // namespace vdd_utils
}  // namespace display_device