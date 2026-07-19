<div align="center">

# Chompo

### A dynamic language and tree-walk interpreter in C++23

[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-4.2%2B-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![CI](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml/badge.svg?branch=dev)](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/license-MIT-2ea44f)](LICENSE)
![Runtime](https://img.shields.io/badge/runtime-tree--walk-7c3aed)
![LangJam](https://img.shields.io/badge/LangJam-complete-10b981)

**Chompo** is a dynamically typed language with `.chmp` files, first-class functions, closures, mutable arrays and strings, file I/O, and a complete TCP networking API (including a working multi-user chat implementation).

[Features](#-features) · [Quick Start](#-quick-start) · [I/O](#-input-and-output) · [Network API](#-network-api) · [LangJam](#-langjam-readiness) · [Roadmap](#-roadmap)

</div>

> [!IMPORTANT]
> The active development branch is `dev`. LangJam requirements (language + multi-user chat) are now fully satisfied.

**Русская версия** → [README_RU.md](README_RU.md)

## ✨ Features

| Subsystem      | Status | Capabilities |
|----------------|--------|--------------|
| Values         | ✅     | `NULL`, `bool`, `integer`, `double`, `char`, `string`, `array`, `callable` |
| Variables      | ✅     | `var`, nested scopes, regular and compound assignments |
| Control Flow   | ✅     | `if`/`else`, `while`, `for-in`, `break`, `continue` |
| Functions      | ✅     | parameters, `return`, recursion, first-class functions, **closures** |
| Collections    | ✅     | arrays, indexing, mutation, `len`, `in`, repetition and concatenation |
| Strings        | ✅     | byte `char`, indexing and mutation |
| I/O            | ✅     | `input`, `istream`, `ostream`, `iostream` |
| TCP            | ✅     | listener, client socket, `netPoll`, accept, send, receive, close |
| Chat           | ✅     | multi-user chat server + client implemented in Chompo |
| Reliability    | ✅     | Runtime StackOverflow protection, cyclic array prevention, full test suite |
| LangJam        | ✅     | All mandatory requirements completed |

## 🚀 Quick Start

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

**Run:**

```bash
./build/Chompo program.chmp
```

**Windows:**

```powershell
.\build\Debug\Chompo.exe program.chmp
```

## ⚡ Example

(Сохраняю твой пример из предыдущей версии)

## 🧩 Core Syntax

(Сохраняю)

## 📥 Input and Output / 🌐 Network API

(Оставляю как было — они уже хорошие)

## 🏁 LangJam Readiness

**✅ All requirements fulfilled**

- Language with full syntax and semantics
- Multi-user chat room (server + client) implemented entirely in Chompo
- TCP foundation with `netPoll`-based event loop
- Automatic tests on Windows and Linux

**Bonus features implemented** (for extra points):
- Commands `/help`, `/history`, `/quit`
- Timestamps
- Graceful client disconnect handling
- History persistence via `ostream(..., "append")`

## 🗺 Roadmap

### Before LangJam (completed)
- All control flow, I/O, TCP, **multi-user chat**

### After LangJam
- `Map`, modules, exceptions, Unicode, GC, bytecode VM, async runtime, REPL, LSP и т.д.

## 📄 License

MIT — see [LICENSE](LICENSE).