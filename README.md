<div align="center">

# Chompo

### Динамический язык и расширяемый высокопроизводительный интерпретатор на C++23

[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-4.2%2B-064F8C?logo=cmake)](https://cmake.org/)
[![CI](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml/badge.svg)](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/license-MIT-2ea44f)](LICENSE)

**Chompo** поддерживает функции первого класса, closures, изменяемые массивы, файловый I/O и неблокирующий TCP API.

[Wiki](docs/wiki/Home.md) · [Синтаксис](docs/wiki/Language-Syntax.md) · [Built-ins](docs/wiki/Built-in-Functions.md) · [Network API](docs/wiki/Network-API.md) · [Runtime](docs/wiki/Runtime-Architecture.md)

</div>

> [!IMPORTANT]
> Ветка `feature/slot-runtime` содержит изолированную максимальную оптимизацию tree-walk runtime. Базовая `feature/perf-wiki-push-pop` остаётся отдельным стабильным слоем с Wiki, `push/pop` и performance checker.

## Возможности

| Подсистема | Поддержка |
|---|---|
| Типы | `NULL`, `bool`, `integer`, `double`, `char`, `string`, `array`, `callable` |
| Управление | `if`, `else`, `while`, `for-in`, `break`, `continue`, `return` |
| Функции | рекурсия, closures, функции как значения |
| Коллекции | индексация, мутация, `len`, `in`, `push`, `pop`, конкатенация, повторение |
| I/O | `input`, `istream`, `ostream`, `iostream` |
| TCP | `netListen`, `netConnect`, `netAccept`, `netPoll`, `netSend`, receive API, close |
| Runtime | Resolver, `SymbolId`, плотные local slots, pools, cached literals, integer fast paths |
| Расширение | независимые native-модули и динамический global registry |
| Проверки | CTest, Windows/Linux CI, execution-only Release TLE suite |

## Сборка

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Release:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel
```

Максимальная локальная оптимизация под текущий CPU:

```bash
cmake -S . -B build-native -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCHOMPO_NATIVE_OPTIMIZATIONS=ON
cmake --build build-native --parallel
```

Для GCC/Clang также доступны `CHOMPO_PGO_GENERATE` и `CHOMPO_PGO_USE` для profile-guided optimization.

Запуск:

```bash
./build/Chompo program.chmp
```

Windows с multi-config генератором:

```powershell
.\build\Debug\Chompo.exe program.chmp
```

## Пример

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
print(counter(), " ", counter(), "\n"); // 11 12
```

## Максимально оптимизированный tree-walk runtime

```text
source -> Lexer -> Parser -> Resolver -> optimized Interpreter
```

Resolver один раз превращает локальное имя в адрес `(depth, slot)`. Во время исполнения локальные переменные и параметры читаются из плотного `vector<Value>` без хеширования строк.

Дополнительные hot-path оптимизации:

- литералы декодируются один раз и кешируются в AST;
- блоки без собственных `var`/`fun` не создают `Environment`;
- block, loop и function environments переиспользуются, если не захвачены closure;
- `return`, `break` и `continue` не используют C++ exceptions;
- обычные присваивания и `++/--` работают напрямую с `Value&`, без `std::function` target;
- `integer × integer` использует отдельный быстрый arithmetic path;
- argument vectors переиспользуются для каждого уровня вложенности вызовов;
- global root кешируется в `Environment`;
- `push` сохраняет геометрический рост `std::vector` и амортизированное O(1);
- Release использует `-O3`/`/O2`, IPO/LTO, dead-section elimination и опциональные native/PGO режимы.

Глобальные и native-значения остаются в расширяемом реестре по `SymbolId`:

```cpp
interpreter.install_collection_builtins();
interpreter.install_io_builtins(io_manager);
interpreter.install_network_builtins(network_manager);
```

Resolver не зависит от конкретных built-ins, I/O или сети. Новые native-модули добавляются без изменения формата slots и runtime hot path.

Подробнее: [`docs/wiki/Runtime-Architecture.md`](docs/wiki/Runtime-Architecture.md).

## Измерения execution-only suite

Release-бинарник из GitHub Actions на пяти тяжёлых сценариях:

| Сценарий | Время |
|---|---:|
| 300 000 арифметических итераций | ~0.051 с |
| 75 000 пользовательских вызовов | ~0.029 с |
| 50 000 `push` + 25 000 `pop` | ~0.026 с |
| 200 000 глубоких scope lookup | ~0.025 с |
| 200 000 вызовов с ранним `return` | ~0.071 с |

До исправления стратегии capacity массивный тест превышал 20 секунд. После восстановления геометрического роста он выполняется примерно за 0.026 секунды на том же типе Release runner-бинарника.

Результаты зависят от CPU и нагрузки runner; checker предназначен прежде всего для обнаружения регрессий.

## Execution-only TLE checker

```bash
cmake -S . -B build-perf -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCHOMPO_ENABLE_PERFORMANCE_TESTS=ON
cmake --build build-perf --parallel
ctest --test-dir build-perf -L performance --output-on-failure
```

В CI Release-бинарник собирается отдельно. Индивидуальные TLE-лимиты включают только запуск интерпретатора на `.chmp`, а не CMake и C++-компиляцию.

## `push` и `pop`

```javascript
var values = Array{};
push(values, 10, 20, 30);
print(pop(values), "\n"); // 30
```

`push(array, values...)` мутирует массив и возвращает новую длину. `pop(array)` удаляет последний элемент; пустой массив возвращает `NULL`.

## LangJam

Chompo уже является полноценным интерпретатором на C++. Отдельная VM для допуска не требуется. Следующий продуктовый этап — сервер и клиент многопользовательского чата на самом Chompo.

## Лицензия

MIT — см. [LICENSE](LICENSE).
