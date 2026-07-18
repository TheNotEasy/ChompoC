# Runtime и производительность

Chompo остаётся tree-walk интерпретатором, но runtime оптимизирован для частых операций.

## Реализованные оптимизации

- идентификаторы интернируются lexer-ом в числовые `SymbolId`;
- Environment ищет переменные по `uint32_t`, а не повторно хеширует строки;
- глубина найденной переменной кешируется внутри активного scope;
- пользовательские функции переиспользуют завершённые call frames;
- frame не переиспользуется, когда удерживается closure, поэтому семантика замыканий сохраняется;
- lexer заранее резервирует память под tokens;
- keyword lookup выполняется через `string_view`;
- Release включает IPO/LTO, когда компилятор его поддерживает;
- `push` использует `reserve`, `pop` перемещает последний элемент.

## Performance/TLE suite

Тяжёлый checker запускается только на Release-сборке:

```bash
cmake -S . -B build-perf -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCHOMPO_ENABLE_PERFORMANCE_TESTS=ON
cmake --build build-perf --parallel
ctest --test-dir build-perf -L performance --output-on-failure
```

Набор проверяет арифметику, вызовы функций, массивы и lookup через глубокие scope. Каждый сценарий одновременно проверяет checksum и ограничение времени. Увеличение лимита не считается исправлением регрессии: сначала должен быть найден и устранён hot path.

## Ограничения runtime

- максимальная глубина вызовов задаётся `ChompoConfig::MaxCallDepth`;
- циклические ссылки массивов запрещены;
- строки и `char` работают с байтами, не с Unicode code points;
- массивы имеют ссылочную семантику;
- `for-in` обходит snapshot последовательности.
