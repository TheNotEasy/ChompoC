# Синтаксис языка

## Переменные

```javascript
var value = 10;
var empty;
value = 20;
value += 2;
value -= 1;
value *= 3;
value /= 2;
```

Неинициализированная переменная получает `NULL`. Повторное объявление имени в одном scope запрещено. Вложенный блок может скрыть внешнее имя.

## Условия

```javascript
if (value > 10) {
    print("large\n");
} else {
    print("small\n");
}
```

## While

```javascript
while (condition) {
    if (skip)
        continue;
    if (stop)
        break;
}
```

`break` и `continue` разрешены только внутри цикла и не могут перескочить границу вложенной функции.

## For-in

```javascript
for (var element in Array{1, 2, 3})
    print(element, "\n");

for (var character in "abc")
    print(character, "\n");
```

Итерируемое выражение вычисляется один раз. Массив обходится по snapshot-копии; строка — по байтовым `char`. Переменная каждой итерации живёт в отдельном scope.

## Функции

```javascript
fun add(left, right) {
    return left + right;
}
```

`return;` возвращает `NULL`. Функция без явного `return` также возвращает `NULL`.

## Print

```javascript
print("value=", value, "\n");
```

`print` является инструкцией, принимает ноль или несколько выражений и не добавляет перевод строки автоматически.

## Литералы

```javascript
NULL
true
false
123
3.14
'A'
"text\n"
Array{1, 2, 3}
```

Поддерживаемые escape-последовательности строк: `\n`, `\t`, `\r`, `\"`, `\\`. Для `char`: `\n`, `\t`, `\r`, `\0`, `\\`, `\'`.
