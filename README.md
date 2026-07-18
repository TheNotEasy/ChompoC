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
> Рабочая ветка проекта — `dev`. Эта feature-ветка добавляет `push/pop`, оптимизации, Wiki и performance/TLE checker. Собственная VM для LangJam не требуется.

## Возможности

| Подсистема | Поддержка |
|---|---|
| Типы | `NULL`, `bool`, `integer`, `double`, `char`, `string`, `array`, `callable` |
| Управление | `if`, `else`, `while`, `for-in`, `break`, `continue`, `return` |
| Функции | рекурсия, closures, функции как значения |
| Коллекции | индексация, мутация, `len`, `in`, `push`, `pop`, конкатенация, повторение |
| I/O | `input`, `istream`, `ostream`, `iostream` |
| TCP | `netListen`, `netConnect`, `netAccept`, `netPoll`, `netSend`, receive API, close |
| Проверки | CTest, Windows/Linux CI, Release performance/TLE suite |

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

var length = push(values, 1, 2, 3); // 3
var last = pop(values);             // 3
var empty = pop(Array{});           // NULL
```

- `push(array, values...)` мутирует массив и возвращает новую длину;
- `pop(array)` удаляет и возвращает последний элемент;
- массивы имеют ссылочную семантику, поэтому изменение видно через aliases;
- создание циклических ссылок запрещено и проверяется до изменения массива.

## Performance/TLE checker

Тяжёлые проверки запускаются отдельно на Release-сборке:

```bash
cmake -S . -B build-perf \
  -DCMAKE_BUILD_TYPE=Release \
  -DCHOMPO_ENABLE_PERFORMANCE_TESTS=ON
cmake --build build-perf --parallel
ctest --test-dir build-perf -L performance --output-on-failure
```

Checker выполняет тяжёлую арифметику, частые вызовы функций, глубокий lookup scope и массовые `push/pop`. Он сверяет контрольные суммы и завершает тест с `TLE`, если сценарий превышает лимит.

## Оптимизации этой ветки

- Release IPO/LTO, когда поддерживается компилятором;
- предупреждения компилятора `/W4` или `-Wall -Wextra -Wpedantic`;
- чтение исходного файла одной заранее выделенной строкой;
- отключение синхронизации C++ streams с C stdio;
- предварительный `reserve` для global/local environments;
- `push` вместо квадратичной конкатенации `array += Array{value}`;
- отдельный Release performance job в GitHub Actions.

## Документация

Полная документация находится в [`docs/wiki`](docs/wiki/Home.md) и включает:

- весь синтаксис;
- типы, truthiness и операторы;
- функции, closures и scope;
- массивы и строки;
- каждую встроенную функцию;
- файловый I/O;
- весь Network API;
- ошибки, ограничения и performance checker.

Файлы имеют формат GitHub Wiki (`Home.md`, `_Sidebar.md`). После включения Wiki их можно перенести в `ChompoC.wiki.git` без изменения структуры.

## LangJam

Tree-walk интерпретатор на C++ уже удовлетворяет требованию реализации языка. VM и AtomVM не обязательны. До сдачи остаются главным образом сервер и клиент чата на Chompo, broadcast, последние N сообщений, корректное отключение и инструкция запуска.

## Лицензия

MIT — см. [LICENSE](LICENSE).
