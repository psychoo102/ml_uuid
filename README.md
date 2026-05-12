# ml_uuid 💎

![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey)
![License](https://img.shields.io/badge/license-MIT-blue)
![C++](https://img.shields.io/badge/language-C%2B%2B17-orange)
![MTA:SA](https://img.shields.io/badge/MTA%3ASA-Module-brightgreen)

`ml_uuid` is a high-performance C++ module for **Multi Theft Auto: San Andreas** that provides fast, reliable, and modern UUID generation.  
It is built with cross-platform CMake Presets and offers both random (v4) and time-sortable (v7) identifiers.

---

## 🚀 Why ml_uuid?

Lua-based UUID generators often rely on weak randomness or slow math libraries.  
`ml_uuid` solves this by using **Boost::uuid** and native C++17 performance:

✔️ **uuidv4()** – Cryptographically strong random identifiers  
✔️ **uuidv7()** – Modern, time-ordered UUIDs standardized in 2024  
✔️ **Cross-platform** – Windows & Linux  
✔️ **Lightweight** – No external runtime dependencies  
✔️ **Drop-in module** for any MTA server

---

## 📈 The UUIDv7 Advantage

UUIDv7 includes a timestamp component, meaning values are **monotonically increasing**.

This makes it perfect for:

* Database primary keys  
* Event logs  
* Distributed systems  
* Any place where you want both uniqueness and natural ordering

Compared to `uuidv4()`, using `uuidv7()` for database inserts:

* Reduces B-tree fragmentation  
* Improves long-term indexing performance  
* Speeds up inserts on large tables  

---

## 🛠️ Installation

1. Download the latest release from the **[Releases](../../releases)** page.
2. Place the module file in your MTA server:
   * **Windows:**  
     `server/mods/deathmatch/modules/ml_uuid.dll`
   * **Linux:**  
     `server/mods/deathmatch/modules/ml_uuid.so`
3. Enable the module in `mtaserver.conf`:

```xml
<module src="ml_uuid.dll" /> <!-- Use ml_uuid.so on Linux -->
```

## 📖 Lua API Reference

```lua
-- uuidv4()
-- Generates a random RFC 4122 Version 4 UUID.
-- Returns: string (example: f47ac10b-58cc-4372-a567-0e02b2c3d479)

local sessionToken = uuidv4()


-- uuidv7()
-- Generates a sortable Version 7 UUID using timestamps and entropy.
-- Returns: string (example: 018c2f1a-5c21-7b18-8b21-4f3b25687a41)

local userId = uuidv7()
dbExec(conn, "INSERT INTO players (uid, name) VALUES (?, ?)", userId, "PlayerName")
```

## 🧪 Building From Source

This project uses **CMake** with presets for both Windows and Linux.

Windows:

```bash
cmake --preset win32-release
cmake --build --preset win32-release
```

Linux:

```bash
cmake --preset linux-x64-release
cmake --build --preset linux-x64-release
```

### Requirements:

C++17
Boost (UUID)
vcpkg (handled automatically)

All presets are already included in the repository for convenience.

## 🤝 Contributing

Contributions are welcome, whether it’s code, documentation, or improvements to the build system.
Feature suggestions and bug reports with reproducible steps are especially appreciated.

## ⭐ Support the Project

If this module improved your server or workflow, consider starring the repository.
It helps keep the project alive and discoverable.

## 📜 License

Released under the MIT License.
You may freely use it in both commercial and non-commercial projects.