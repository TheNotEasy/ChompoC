<div align="center">

# Chompo

### A dynamic language, optimized interpreter, and multi-user TCP chat in C++23

[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-4.2%2B-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![CI](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/license-MIT-2ea44f)](LICENSE)
![Runtime](https://img.shields.io/badge/runtime-optimized_tree--walk-7c3aed)
![LangJam](https://img.shields.io/badge/LangJam-chat_ready-2ea44f)

**Chompo** is a dynamically typed language with first-class functions, closures, mutable arrays and byte strings, file I/O, command-line arguments, and a non-blocking TCP API.

[GitHub Wiki](https://github.com/Bony-Lord/ChompoC/wiki) · [Syntax](docs/wiki/Language-Syntax.md) · [Built-ins](docs/wiki/Built-in-Functions.md) · [Network API](docs/wiki/Network-API.md) · [LangJam Chat](docs/wiki/LangJam-Chat.md)

</div>

**Русская версия → [README_RU.md](README_RU.md)**

> [!IMPORTANT]
> The canonical project branch is `main`. GitHub Wiki pages are synchronized automatically from `docs/wiki` only for `origin/main`.

## Features

| Subsystem | Capabilities |
|---|---|
| Values | `NULL`, `bool`, `integer`, `double`, `char`, `string`, `array`, `callable` |
| Control flow | `if`, `else`, `while`, `for-in`, `break`, `continue`, `return` |
| Functions | recursion, closures, first-class callable values |
| Collections | indexing, mutation, `len`, `in`, `push`, `pop`, `removeAt`, concatenation, repetition |
| I/O | `input`, `inputPoll`, `flush`, `istream`, `ostream`, `iostream` |
| System | `args()` for Chompo program arguments |
| TCP | listener/client sockets, `netPoll`, partial send, `netSendAll`, receive statuses, close |
| Runtime | Resolver, `SymbolId`, dense local slots, environment pools, cached literals, integer fast paths |
| Chat | unique names, broadcast, bounded history, `/help`, `/history`, `/quit`, disconnect cleanup |
| Verification | Windows/Linux CTest, TCP loopback, end-to-end chat test, execution-only Release TLE suite |

## Build

A C++23 compiler and CMake 4.2+ are required.

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Release build:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel
```

Run a Chompo program:

```bash
./build/Chompo program.chmp argument1 argument2
```

`args()` returns only the arguments placed after the `.chmp` file name.

Windows with a multi-config generator:

```powershell
.\build\Debug\Chompo.exe program.chmp argument1 argument2
```

## Language example

```javascript
fun makeCounter(start) {
    var value = start;

    fun next() {
        value++;
        return value;
    }

    return next;
}

var counter = makeCounter(10);
var values = Array{counter(), counter()};
push(values, 13);
print(values, "\n"); // {11, 12, 13}
```

## LangJam chat

Start the server:

```bash
./build/Chompo langjam/Chompo/chat_server.chmp 0.0.0.0 4040 50
```

Optional arguments are `host`, `port`, and `historyLimit`. Port `0` asks the operating system to choose a free port and the server prints it as `LISTENING <port>`.

Start one or more clients:

```bash
./build/Chompo langjam/Chompo/chat_client.chmp 127.0.0.1 4040
```

Windows:

```powershell
.\build\Debug\Chompo.exe langjam\Chompo\chat_server.chmp 0.0.0.0 4040 50
.\build\Debug\Chompo.exe langjam\Chompo\chat_client.chmp 127.0.0.1 4040
```

Client commands:

```text
/help       show available commands
/history    receive the current bounded history
/quit       leave the chat
```

The server and client are written entirely in Chompo. C++ provides the interpreter and host APIs for TCP and streams.

## Chat-ready API

```javascript
var arguments = args();
var clients = Array{};
push(clients, socket);
removeAt(clients, 0);

var console = inputPoll(0);
// {"data", line}, {"wait"}, {"closed"}

flush();

var sent = netSendAll(socket, "hello\n", 2000);
// {"sent", bytes}, {"timeout", bytes}, {"error", bytes, message}
```

`netSend` is a low-level non-blocking operation and may send only part of a string. Protocol messages should use `netSendAll`.

`netReceiveLine` returns one of:

```javascript
Array{"data", line}
Array{"wait"}
Array{"closed"}
Array{"error", message}
```

See the complete [Network API reference](docs/wiki/Network-API.md).

## Runtime architecture

```text
source -> Lexer -> Pratt Parser -> Resolver -> optimized Interpreter
```

The Resolver converts local names into `(depth, slot)` addresses once. At runtime, local variables are read from dense slots without repeated string hashing.

Additional hot-path optimizations include:

- decoded literals cached in the AST;
- no environment allocation for blocks without local declarations;
- reusable block, loop, and function environments;
- `return`, `break`, and `continue` without C++ exceptions;
- direct assignment and update targets;
- specialized integer arithmetic paths;
- reusable argument vectors;
- amortized O(1) `push`;
- Release `-O3`/`/O2`, IPO/LTO, and optional native/PGO modes.

Global values and native functions remain in an extensible `SymbolId` registry, so new modules can be added without changing the local-slot runtime.

See [Runtime Architecture](docs/wiki/Runtime-Architecture.md).

## Testing

```bash
ctest --test-dir build --output-on-failure
```

The suite includes language and error regression tests, file and console I/O, TCP loopback tests, and `langjam_chat`. The end-to-end chat test starts a real Chompo server, connects multiple clients, verifies duplicate-name rejection, broadcast, history, commands, graceful exit, TCP reset cleanup, and launches the actual Chompo client.

GitHub Actions verifies Windows, Ubuntu, Release builds, and the execution-only TLE suite.

## LangJam status

Completed:

- language and C++23 interpreter;
- required values, conditions, loops, functions, recursion, and collections;
- file, console, and TCP APIs;
- multi-user server and client written in Chompo;
- unique user names and retry after rejection;
- message broadcast;
- bounded last-N message history;
- `/help`, `/history`, and `/quit`;
- cleanup after normal and abrupt disconnects;
- launch instructions and language description;
- automated end-to-end verification.

The submission package is located in [`langjam/Chompo`](langjam/Chompo).

## Roadmap

Possible post-jam additions:

- maps/dictionaries;
- modules and `import`;
- language-level exceptions;
- Unicode text;
- cycle-aware garbage collection;
- optional bytecode backend;
- REPL, formatter, LSP, and editor integrations.

## License

MIT — see [LICENSE](LICENSE).
