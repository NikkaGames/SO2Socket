
# SO2Socket

**SO2Socket** is a native C++ module targeting Android environments, designed to extend Unity-based mobile game functionality via memory patching, runtime hooking, encrypted communication, and real-time ESP overlays. Built using a combination of `KittyMemory`, `Dobby`, `xhook`, `xdl`, and custom vector/memory manipulation routines, the project leverages JNI and IL2CPP internals for seamless in-process enhancements.

## Key Features

- 🔍 **ESP Overlay Engine** – Detects player positions and renders on-screen overlays via direct framebuffer coordinate projection.
- 🎯 **Aimbot Core** – Computes directional vectors using quaternion to Euler conversion, targeting the closest valid enemy object.
- 🔐 **Obfuscated Execution** – OLLVM optimized, String encryption, function splitting, and annotation-based obfuscation strategies.
- ⚙️ **Memory Manipulation** – Runtime memory patching through `KittyMemory`, including instruction patching, NOP injection, and dynamic hook registration.
- 📡 **Encrypted UDP Data Streaming** – ESP state is serialized with RapidJSON, XOR-encrypted, and transmitted over UDP sockets.
- 🧠 **Custom IL2CPP Resolver** – Full method/class/field resolution using IL2CPP runtime metadata.
- 🎮 **Modular Weapon and Inventory Enhancer** – Add all skins/items conditionally from JSON or defaults via dynamic dictionary injection.

## Architecture

```
+--------------------+
| JNI Entry (JavaVM) |
+---------+----------+
          |
          v
       (maybe?)
+-----------------------------+
| Native Thread Initialization|
+-----------------------------+
          |
          v
+------------------------+
| Unity Runtime Resolver |
+------------------------+
          |
          v
+---------------------------+
| Game Object Manipulation  |
+---------------------------+
          |
          v
+------------------------------+
| ESP/Aimbot/Inventory Modules |
+------------------------------+
          |
          v
+----------------------+
| UDP Socket Encrypted |
+----------------------+
```

## Dependencies

- Android NDK (C++17)
- [KittyMemory](https://github.com/MJx0/KittyMemory)
- [Dobby](https://github.com/jmpews/Dobby)
- [xhook](https://github.com/iqiyi/xhook)
- [xdl](https://github.com/hexhacking/xdl)
- [RapidJSON](https://github.com/Tencent/rapidjson)
- Custom headers for Unity structs (`Vector3`, `Quaternion`, etc.)
- IL2CPP runtime symbols

## Build Instructions

```bash
# Ensure Android NDK environment is set
Build with Android Studio (NDK r24 recommended)
```

## Usage

1. Inject compiled `.so` into Unity game process using a loader or Zygisk module.
2. Confirm `Unity` and `IL2CPP` libraries are loaded.
3. Initialize hooks, ESP manager, and worker threads.
4. Launch your ESP listener on UDP port `19133`.

## Security Considerations

- All strings are statically obfuscated (`OBFUSCATE` macro).
- Heavy use of C++ template-based junk code (`Bloat<N>`) and infinite recursion traps (`recurseForever`) as anti-analysis.
- Hardcoded integrity and tamper checks via `checkc()` function.
- Uses system properties and `/system/etc/hosts` contents to identify "debug" environments.

## License

This project is proprietary and for educational and research use only. illegal use, redistribution and commercial usage is strictly prohibited.
