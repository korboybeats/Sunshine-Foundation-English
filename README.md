# Sunshine Base

Based on LizardByte/Sunshine, providing complete documentation support [Read the Docs](https://docs.qq.com/aio/DSGdQc3htbFJjSFdO?p=YTpMj5JNNdB5hEKJhhqlSB).

**Sunshine-Foundation** is a self-hosted game stream host for Moonlight, this branch version has undergone significant improvements over the original Sunshine, focusing on improving the game streaming experience from various streaming devices to the Windows host:

### 🌟 Core Features
- **HDR Friendly Support** - Optimized HDR processing pipeline, providing a genuine HDR game streaming experience.
- **Integrated Virtual Display** - Built-in virtual display management, allowing you to create and manage virtual displays without additional software.
- **Remote Microphone Support** - Supports receiving client microphones, providing high-quality voice direct-through functionality.
- **Advanced Control Panel** - Intuitive Web control interface, providing real-time monitoring and configuration management.
- **Low Latency Transmission** - Optimized encoding processing combined with the latest hardware capabilities.
- **Smart Pairing** - Intelligently manages the pairing of devices and their corresponding configuration files.

### 🖥️ Virtual Display Integration (requires Windows 10 22H2 and later systems)
- Dynamic virtual display creation and destruction
- Custom resolution and refresh rate support
- Multi-display configuration management
- Real-time configuration changes without rebooting

## Recommended Moonlight Clients

We recommend using the following optimized Moonlight clients to achieve the best streaming experience (activated package properties):
### 🖥️ Windows(x86_64, Arm64), MacOS, Linux Client
[![Moonlight-PC](https://img.shields.io/badge/Moonlight-PC-red?style=for-the-badge&logo=windows)](https://github.com/qiin2333/moonlight-qt)

### 📱 Android Client
[![Power Enhance Version Moonlight-Android](https://img.shields.io/badge/Power%20Enhance%20Version-Moonlight--Android-green?style=for-the-badge&logo=android)](https://github.com/qiin2333/moonlight-android/releases/tag/shortcut)
[![Crown Version Moonlight-Android](https://img.shields.io/badge/Crown%20Version-Moonlight--Android-blue?style=for-the-badge&logo=android)](https://github.com/WACrown/moonlight-android)

### 📱 iOS Client
[![True Brick Master Version Moonlight-iOS](https://img.shields.io/badge/True%20Brick%20Master%20Version-Moonlight--iOS-lightgrey?style=for-the-badge&logo=apple)](https://github.com/TrueZhuangJia/moonlight-ios-NativeMultiTouchPassthrough)

### 🛠️ Other Resources
[awesome-sunshine](https://github.com/LizardByte/awesome-sunshine)

## System Requirements
> [!WARNING] 
> These tables are continuously updated. Do not base your hardware purchases solely on this information.

<table>
    <caption id="minimum_requirements">Minimum Configuration Requirements</caption>
    <tr>
        <th>Component</th>
        <th>Requirement</th>
    </tr>
    <tr>
        <td rowspan="3">GPU</td>
        <td >AMD: VCE 1.0 or higher version, see: <a href="https://github.com/obsproject/obs-amd-encoder/wiki/Hardware-Support">obs-amd hardware support</a></td>
    </tr>
    <tr>
        <td >Intel: VAAPI compatible, see: <a href="https://www.intel.com/content/www/us/en/developer/articles/technical/linuxmedia-vaapi.html">VAAPI hardware support</a></td>
    </tr>
    <tr>
        <td >Nvidia: Graphics card with NVENC support, see: <a href="https://developer.nvidia.com/video-encode-and-decode-gpu-support-matrix-new">nvenc support matrix</a></td>
    </tr>
    <tr>
        <td rowspan="2">CPU</td>
        <td >AMD: Ryzen 3 or higher</td>
    </tr>
    <tr>
        <td >Intel: Core i3 or higher</td>
    </tr>
    <tr>
        <td >RAM</td>
        <td >4GB or more</td>
    </tr>
    <tr>
        <td rowspan="5">Operating System</td>
        <td >Windows: 10 22H2+ (Windows Server does not support virtual game controller)</td>
    </tr>
    <tr>
        <td >macOS: 12+</td>
    </tr>
    <tr>
        <td >Linux/Debian: 12+ (bookworm)</td>
    </tr>
    <tr>
        <td >Linux/Fedora: 39+</td>
    </tr>
    <tr>
        <td >Linux/Ubuntu: 22.04+ (jammy)</td>
    </tr>
    <tr>
        <td rowspan="2">Network</td>
        <td >Host: 5GHz, 802.11ac</td>
    </tr>
    <tr>
        <td >Client: 5GHz, 802.11ac</td>
    </tr>
</table>

<table>
    <caption id="4k_suggestions">4K Recommended Configuration</caption>
    <tr>
        <th>Component</th>
        <th>Requirement</th>
    </tr>
    <tr>
        <td rowspan="3">GPU</td>
        <td >AMD: Video Coding Engine 3.1 or higher</td>
    </tr>
    <tr>
        <td >Intel: HD Graphics 510 or higher</td>
    </tr>
    <tr>
        <td >Nvidia: GeForce GTX 1080 or higher with multi-encoder models</td>
    </tr>
    <tr>
        <td rowspan="2">CPU</td>
        <td >AMD: Ryzen 5 or higher</td>
    </tr>
    <tr>
        <td >Intel: Core i5 or higher</td>
    </tr>
    <tr>
        <td >Network</td>
        <td >Host: CAT5e Ethernet or better</td>
    </tr>
    <tr>
        <td >Client</td>
        <td >Client: CAT5e Ethernet or better</td>
    </tr>
</table>

## Technical Support

1. Check [User Guide](https://docs.qq.com/aio/DSGdQc3htbFJjSFdO?p=YTpMj5JNNdB5hEKJhhqlSB) [LizardByte documentation](https://docs.lizardbyte.dev/projects/sunshine/latest/)
2. Turn on detailed log level in settings to find relevant information
3. Join QQ group for help: [Get Help via QQ Group](https://qm.qq.com/cgi-bin/qm/qr?k=5qnkzSaLIrIaU4FvumftZH_6Hg7fUuLD&jump_from=webapi)
4. Use Two Letters!: [Visit website](https://uuyc.163.com/)

**Problem feedback tags:**
- `hdr-support` - HDR-related issues
- `virtual-display` - Virtual display problems
- `config-help` - Configuration questions

## Join the Community

We welcome everyone to join and contribute!
[![Join QQ Group](https://pub.idqqimg.com/wpa/images/group.png 'Join QQ group')](https://qm.qq.com/cgi-bin/qm/qr?k=WC2PSZ3Q6Hk6j8U_DG9S7522GPtItk0m&jump_from=webapi&authKey=zVDLFrS83s/0Xg3hMbkMeAqI7xoHXaM3sxZIF/u9JW7qO/D8xd0npytVBC2lOS+z)
## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=qiin2333/Sunshine-Foundation&type=Date)](https://www.star-history.com/#qiin2333/Sunshine-Foundation&Date)
---

**Sunshine Base - Making Game Streaming Simpler**
