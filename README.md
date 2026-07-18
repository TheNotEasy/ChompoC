<div align="center">

# Chompo

### Динамический язык и tree-walk интерпретатор на C++23

[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-4.2%2B-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![CI](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml/badge.svg)](https://github.com/Bony-Lord/ChompoC/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/license-MIT-2ea44f)](LICENSE)

**Chompo** — динамически типизированный язык с функциями первого класса, замыканиями, изменяемыми массивами и строками, файловым I/O и TCP API.

[Wiki](docs/wiki/Home.md) · [Синтаксис](docs/wiki/Language-Syntax.md) · [Все встроенные функции](docs/wiki/Built-in-Functions.md) · [Network API](docs/wiki/Network-API.md) · [Performance](docs/wiki/Runtime-and-Performance.md)

</div>

> [!IMPORTANT]
> Рабочая ветка проекта — `dev`. Feature-ветка `feature/perf-wiki-push-pop` содержит Wiki, `push/pop`, TLE-checker и оптимизированный runtime. Собственная VM для LangJam не требуется.

## Возможности

| Подсистема | Поддержка |
|---|---|
| Типы | `NULL`, `bool`, `integer`, `double`, `char`, `string`, `array`, `callable` |
| Управление | `if`, `else`, `while`, `for-in`, `break`, `continue`, `return` |
| Функции | рекурсия, closures, функции как значения |
| Коллекции | индексация, мутация, `len`, `in`, `push`, `pop`, конкатенация, повторение |
| I/O | `input`, `istream`, `ostream`, `iostream` |
| TCP | `netListen`, `netConnect`, `netAccept`, `netPoll`, `netSend`, receive API, close |
| Проверки | CTest, Windows/Linux CI, Release execution-only TLE suite |

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
fun sum(values) {
    var result = 0;

    for (var value in values)
        result += value;

    return result;
}

var values = Array{};
push(values, 10, 20, 30);

print(sum(values), "\n"); // 60
print(pop(values), "\n"); // 30
```

## `push` и `pop`

```javascript
var values = Array{};

push(values, 10);
push(values, 20, 30);

print(values, "\n"); // {10, 20, 30}
print(pop(values), "\n"); // 30
```

`push(array, values...)` мутирует массив и возвращает новую длину. `pop(array)` удаляет последний элемент; пустой массив возвращает `NULL`.

## Оптимизированный runtime

Текущая feature-ветка ускоряет hot paths без VM:

- имена интернируются в числовые `SymbolId`;
- Environment работает по `uint32_t` вместо повторного хеширования строк;
- глубина lookup кешируется внутри scope;
- завершённые function frames переиспользуются, если не удерживаются closure;
- lexer использует `string_view` для keywords и заранее резервирует tokens;
- Release использует IPO/LTO при поддержке компилятором;
- исходный файл читается одним выделением памяти.

Семантика closures сохраняется: захваченный frame никогда не возвращается в пул.

## Execution-only TLE checker

CI не включает configure, компиляцию и линковку в TLE:

1. job `performance-build` отдельно собирает Release-бинарник и загружает его как artifact;
2. job `performance-execution` скачивает уже готовый бинарник;
3. Python-checker замеряет только время процесса `Chompo <case>.chmp`.

Локальный запуск после уже выполненной Release-сборки:

```bash
python tests/performance/run_performance_suite.py \
  --executable build-release/Chompo \
  --cases tests/performance/cases
```

На Windows:

```powershell
python tests/performance/run_performance_suite.py `
  --executable build-release/Chompo.exe `
  --cases tests/performance/cases
```

Checker запускает тяжёлые программы для:

- арифметики и циклов;
- пользовательских функций;
- массовых `push/pop`;
- lookup через глубокую цепочку scope.

Для каждого сценария проверяется время исполнения и checksum. Установка CMake, сборка C++, artifact upload/download и запуск Python не входят в индивидуальный TLE.

## Документация

Полная документация находится в [`docs/wiki`](docs/wiki/Home.md):

- синтаксис и приоритет операторов;
- типы и преобразования;
- функции и closures;
- массивы и строки;
- все встроенные функции;
- I/O и файловые режимы;
- полный Network API;
- runtime, ошибки и performance suite.

## LangJam

Chompo уже является интерпретатором на C++, поэтому отдельная VM не обязательна. Для сдачи остаётся написать многопользовательский чат на самом Chompo, добавить историю последних сообщений и инструкцию запуска.

## Лицензия

MIT — см. [LICENSE](LICENSE).
