<div align="center">

# Chompo

### Динамический язык, оптимизированный интерпретатор и многопользовательский TCP-чат на C++23

[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-4.2%2B-064F8C?logo=cmake)](https://cmake.org/)
[![CI](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/license-MIT-2ea44f)](LICENSE)
![Runtime](https://img.shields.io/badge/runtime-optimized_tree--walk-7c3aed)
![LangJam](https://img.shields.io/badge/LangJam-chat_ready-2ea44f)

**Chompo** — динамически типизированный язык с функциями первого класса, замыканиями, изменяемыми массивами и байтовыми строками, файловым I/O, аргументами командной строки и неблокирующим TCP API.

[GitHub Wiki](https://github.com/Bony-Lord/ChompoC/wiki) · [Синтаксис](docs/wiki/Language-Syntax.md) · [Built-ins](docs/wiki/Built-in-Functions.md) · [Network API](docs/wiki/Network-API.md) · [LangJam Chat](docs/wiki/LangJam-Chat.md)

</div>

**English version → [README.md](README.md)**

> [!IMPORTANT]
> Основная ветка проекта — `main`. Страницы GitHub Wiki автоматически синхронизируются из `docs/wiki` только для `origin/main`.

## Возможности

| Подсистема | Возможности |
|---|---|
| Значения | `NULL`, `bool`, `integer`, `double`, `char`, `string`, `array`, `callable` |
| Управление | `if`, `else`, `while`, `for-in`, `break`, `continue`, `return` |
| Функции | рекурсия, closures, функции как значения |
| Коллекции | индексация, мутация, `len`, `in`, `push`, `pop`, `removeAt`, конкатенация, повторение |
| I/O | `input`, `inputPoll`, `flush`, `istream`, `ostream`, `iostream` |
| Система | `args()` для аргументов Chompo-программы |
| TCP | listener/client sockets, `netPoll`, partial send, `netSendAll`, receive statuses, close |
| Runtime | Resolver, `SymbolId`, плотные local slots, environment pools, cached literals, integer fast paths |
| Чат | уникальные имена, broadcast, ограниченная история, `/help`, `/history`, `/quit`, очистка отключений |
| Проверки | Windows/Linux CTest, TCP loopback, end-to-end chat test, execution-only Release TLE suite |

## Сборка

Требуются компилятор с поддержкой C++23 и CMake 4.2+.

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Release-сборка:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel
```

Запуск Chompo-программы:

```bash
./build/Chompo program.chmp argument1 argument2
```

`args()` возвращает только аргументы, записанные после имени `.chmp`-файла.

Windows с multi-config генератором:

```powershell
.\build\Debug\Chompo.exe program.chmp argument1 argument2
```

## Пример языка

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

## LangJam Chat

Запуск сервера:

```bash
./build/Chompo langjam/Chompo/chat_server.chmp 0.0.0.0 4040 50
```

Необязательные аргументы: `host`, `port`, `historyLimit`. Порт `0` просит операционную систему выбрать свободный порт, после чего сервер выводит `LISTENING <port>`.

Запуск одного или нескольких клиентов:

```bash
./build/Chompo langjam/Chompo/chat_client.chmp 127.0.0.1 4040
```

Windows:

```powershell
.\build\Debug\Chompo.exe langjam\Chompo\chat_server.chmp 0.0.0.0 4040 50
.\build\Debug\Chompo.exe langjam\Chompo\chat_client.chmp 127.0.0.1 4040
```

Команды клиента:

```text
/help       показать доступные команды
/history    получить текущую ограниченную историю
/quit       выйти из чата
```

Сервер и клиент полностью написаны на Chompo. C++ предоставляет интерпретатор и host API для TCP и потоков.

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

`netSend` — низкоуровневая неблокирующая операция, которая может отправить только часть строки. Для сообщений протокола следует использовать `netSendAll`.

`netReceiveLine` возвращает один из вариантов:

```javascript
Array{"data", line}
Array{"wait"}
Array{"closed"}
Array{"error", message}
```

Полное описание: [Network API](docs/wiki/Network-API.md).

## Архитектура runtime

```text
source -> Lexer -> Pratt Parser -> Resolver -> optimized Interpreter
```

Resolver один раз переводит локальные имена в адреса `(depth, slot)`. Во время исполнения локальные переменные читаются из плотных slots без повторного хеширования строк.

Дополнительные оптимизации hot path:

- декодированные литералы кешируются в AST;
- блоки без локальных объявлений не создают окружение;
- block, loop и function environments переиспользуются;
- `return`, `break` и `continue` не используют C++ exceptions;
- прямые assignment и update targets;
- специализированные пути целочисленной арифметики;
- повторное использование argument vectors;
- амортизированное O(1) для `push`;
- Release `-O3`/`/O2`, IPO/LTO и опциональные native/PGO режимы.

Глобальные значения и native-функции остаются в расширяемом реестре `SymbolId`, поэтому новые модули добавляются без изменения runtime локальных slots.

Подробнее: [Runtime Architecture](docs/wiki/Runtime-Architecture.md).

## Тестирование

```bash
ctest --test-dir build --output-on-failure
```

Набор включает языковые и error regression tests, файловый и консольный I/O, TCP loopback tests и `langjam_chat`. End-to-end тест поднимает настоящий сервер Chompo, подключает нескольких клиентов, проверяет отказ занятого имени, broadcast, историю, команды, обычный выход, очистку после TCP reset и запуск настоящего Chompo-клиента.

GitHub Actions проверяет Windows, Ubuntu, Release-сборку и execution-only TLE suite.

## Статус LangJam

Готово:

- язык и интерпретатор на C++23;
- необходимые значения, условия, циклы, функции, рекурсия и коллекции;
- файловый, консольный и TCP API;
- многопользовательский сервер и клиент на Chompo;
- уникальные имена и повторная попытка после отказа;
- broadcast сообщений;
- ограниченная история последних N сообщений;
- `/help`, `/history`, `/quit`;
- очистка после обычных и резких отключений;
- инструкции запуска и описание языка;
- автоматическая end-to-end проверка.

Submission package находится в [`langjam/Chompo`](langjam/Chompo).

## Roadmap

Возможные дополнения после jam:

- Map/словари;
- модули и `import`;
- exceptions уровня языка;
- Unicode-текст;
- garbage collector с поддержкой циклов;
- опциональный bytecode backend;
- REPL, formatter, LSP и интеграции с редакторами.

## Лицензия

MIT — см. [LICENSE](LICENSE).
