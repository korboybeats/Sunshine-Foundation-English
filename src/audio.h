/**
 * @file src/audio.h
 * @brief Declarations for audio capture and encoding.
 */
#pragma once

// local includes
#include "platform/common.h"
#include "thread_safe.h"
#include "utility.h"

#include <bitset>

namespace audio {
  enum stream_config_e : int {
    STEREO,  ///< Stereo
    HIGH_STEREO,  ///< High stereo
    SURROUND51,  ///< Surround 5.1
    HIGH_SURROUND51,  ///< High surround 5.1
    SURROUND71,  ///< Surround 7.1
    HIGH_SURROUND71,  ///< High surround 7.1
    MAX_STREAM_CONFIG  ///< Maximum audio stream configuration
  };

  struct opus_stream_config_t {
    std::int32_t sampleRate;
    int channelCount;
    int streams;
    int coupledStreams;
    const std::uint8_t *mapping;
    int bitrate;
  };

  struct stream_params_t {
    int channelCount;
    int streams;
    int coupledStreams;
    std::uint8_t mapping[8];
  };

  extern opus_stream_config_t stream_configs[MAX_STREAM_CONFIG];

  struct config_t {
    enum flags_e : int {
      HIGH_QUALITY,  ///< High quality audio
      HOST_AUDIO,  ///< Host audio
      CUSTOM_SURROUND_PARAMS,  ///< Custom surround parameters
      MAX_FLAGS  ///< Maximum number of flags
    };

    int packetDuration;
    int channels;
    int mask;

    stream_params_t customStreamParams;

    std::bitset<MAX_FLAGS> flags;
  };

  struct audio_ctx_t {
    // We want to change the sink for the first stream only
    std::unique_ptr<std::atomic_bool> sink_flag;

    std::unique_ptr<platf::audio_control_t> control;

    bool restore_sink;
    platf::sink_t sink;
  };

  using buffer_t = util::buffer_t<std::uint8_t>;
  using packet_t = std::pair<void *, buffer_t>;
  using audio_ctx_ref_t = safe::shared_t<audio_ctx_t>::ptr_t;

  void
  capture(safe::mail_t mail, config_t config, void *channel_data);

  /**
   * @brief Get the reference to the audio context.
   * @returns A shared pointer reference to audio context.
   * @note Aside from the configuration purposes, it can be used to extend the
   *       audio sink lifetime to capture sink earlier and restore it later.
   *
   * @examples
   * audio_ctx_ref_t audio = get_audio_ctx_ref()
   * @examples_end
   */
  audio_ctx_ref_t
  get_audio_ctx_ref();

  /**
   * @brief Check if the audio sink held by audio context is available.
   * @returns True if available (and can probably be restored), false otherwise.
   * @note Useful for delaying the release of audio context shared pointer (which
   *       tries to restore original sink).
   *
   * @examples
   * audio_ctx_ref_t audio = get_audio_ctx_ref()
   * if (audio.get()) {
   *     return is_audio_ctx_sink_available(*audio.get());
   * }
   * return false;
   * @examples_end
   */
  bool
  is_audio_ctx_sink_available(const audio_ctx_t &ctx);

  /**
   * @brief Start the microphone redirect device.
   * @returns 0 on success, -1 on error.
   */
  int
  init_mic_redirect_device();

  /**
   * @brief Release the microphone redirect device.
   */
  void
  release_mic_redirect_device();

  /**
   * @brief Write microphone data to the virtual audio device.
   * @param data Pointer to the audio data.
   * @param size Size of the audio data in bytes.
   * @returns Number of bytes written, or -1 on error.
   */
  int
  write_mic_data(const std::uint8_t *data, size_t size);
}  // namespace audio