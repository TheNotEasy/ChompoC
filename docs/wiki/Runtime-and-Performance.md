# Ошибки, ограничения и производительность

## Категории ошибок

- Lexer error — неизвестный символ, незакрытая строка или неверный `char`.
- Parser error — нарушенная грамматика, неверный target присваивания, `break/continue/return` вне допустимого контекста.
- Runtime error — неверные типы, индекс вне границ, деление на ноль, ошибка файла/сети.

Сообщение содержит позицию токена, если ошибка относится к коду Chompo.

## Runtime StackOverflow

Глубина вызовов ограничена `ChompoConfig::MaxCallDepth`. Бесконечная рекурсия завершается контролируемой ошибкой, а не падением C++-стека.

## Циклические массивы

Массивы имеют ссылочную семантику, но циклические ссылки запрещены:

```javascript
var values = Array{};
push(values, values); // runtime error
```

Операция `push` сначала проверяет все новые элементы и только затем изменяет массив.

## Ограничения

- строки и `char` байтовые, полноценного Unicode пока нет;
- отсутствуют словари, классы, модули и исключения языка;
- `for-in` использует snapshot iterable;
- VM/JIT отсутствуют: интерпретатор исполняет AST напрямую;
- API языка до стабильного релиза может меняться.

## Release и LTO

Release-сборка автоматически включает IPO/LTO, когда это поддерживает toolchain:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel
```

## Performance/TLE checker

Тяжёлые проверки выключены в обычной Debug-сборке. Запуск:

```bash
cmake -S . -B build-perf \
  -DCMAKE_BUILD_TYPE=Release \
  -DCHOMPO_ENABLE_PERFORMANCE_TESTS=ON
cmake --build build-perf --parallel
ctest --test-dir build-perf -L performance --output-on-failure
```

Набор проверяет:

- 300 000 арифметических итераций;
- 75 000 вызовов пользовательской функции;
- 50 000 `push` и 25 000 `pop`;
- 200 000 обращений к переменной через глубокую цепочку scope.

Для каждого сценария проверяется checksum и лимит времени. При превышении выводится `TLE`.

Локальный множитель лимитов:

```bash
python tests/performance/run_performance_suite.py \
  --executable build-release/Chompo \
  --cases tests/performance/cases \
  --timeout-scale 2
```

Performance checker предназначен для обнаружения крупных регрессий, а не для точного сравнения наносекунд между разным железом.
