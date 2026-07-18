# Chompo Wiki

Chompo — динамический язык с tree-walk интерпретатором на C++23. Файлы программ имеют расширение `.chmp`.

## Разделы

- [Быстрый старт](Getting-Started)
- [Синтаксис языка](Language-Syntax)
- [Типы и операторы](Types-and-Operators)
- [Функции и области видимости](Functions-and-Scopes)
- [Массивы и строки](Arrays-and-Strings)
- [Встроенные функции](Built-in-Functions)
- [Ввод, вывод и файлы](Input-and-Output)
- [Network API](Network-API)
- [Ошибки, ограничения и производительность](Runtime-and-Performance)

## Минимальная программа

```javascript
var values = Array{1, 2, 3};
push(values, 4);

for (var value in values)
    print(value, "\n");
```

Chompo использует динамические типы, лексические области видимости, функции первого класса, замыкания, изменяемые массивы, изменяемые по индексу строки, файловый I/O и TCP sockets.
