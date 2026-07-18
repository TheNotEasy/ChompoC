# Ввод и вывод

## `input()`

```javascript
var line = input();
```

Читает одну строку без завершающего `\n` из текущего входного потока. На EOF возвращает `NULL`.

## Стандартный поток

Стандартный ввод/вывод обозначается строкой `"standart"`. Это историческое написание является частью текущего API.

## `istream(path = "standart")`

Меняет источник для `input()`.

```javascript
istream("data.txt");
var first = input();
istream("standart");
```

Повторное открытие файла начинает чтение с начала.

## `ostream(path = "standart", mode = "rewrite")`

Меняет поток, куда пишет инструкция `print`.

```javascript
ostream("result.txt", "rewrite");
print("new\n");

ostream("result.txt", "append");
print("more\n");

ostream("standart");
```

Режимы:

| Режим | Поведение |
|---|---|
| `"rewrite"` | создать или полностью перезаписать файл |
| `"append"` | дописывать в конец |
| `"create"` | создать новый файл; ошибка, если он существует |

## `iostream(inputPath = "standart", outputPath = "standart", outputMode = "rewrite")`

Меняет оба потока за один вызов.

```javascript
iostream("request.txt", "response.txt", "rewrite");
print(input(), "\n");
iostream();
```

## `print(arguments...)`

`print` является инструкцией языка, а не callable-функцией. Она выводит аргументы подряд без автоматических пробелов и перевода строки.

```javascript
print("value=", 42, "\n");
```

Для перевода строки нужно явно передать `"\n"`.
