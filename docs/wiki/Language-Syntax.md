# Быстрый старт

## Требования

- компилятор C++23;
- CMake 4.2+;
- Ninja, Make или генератор IDE.

## Сборка

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

## Запуск

```bash
./build/Chompo program.chmp
```

Windows multi-config:

```powershell
.\build\Debug\Chompo.exe program.chmp
```

## Структура программы

Инструкции обычно заканчиваются `;`. Блоки записываются в `{}`.

```javascript
var name = "Chompo";

fun greet(value) {
    print("Hello, ", value, "\n");
}

greet(name);
```

Комментарии начинаются с `//` и продолжаются до конца строки.
