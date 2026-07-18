# Network API

Network API предоставляет неблокирующие TCP-сокеты через целочисленные handles.

## `netListen(host, port, backlog = 16)`

Создаёт listener и возвращает его handle.

```javascript
var listener = netListen("0.0.0.0", 4040);
```

`port = 0` просит ОС выбрать свободный порт. Его можно узнать через `netPort`.

## `netConnect(host, port)`

Подключается к TCP-серверу и возвращает socket handle.

```javascript
var socket = netConnect("127.0.0.1", 4040);
```

## `netAccept(listener)`

Возвращает новый socket handle или `NULL`, если ожидающих подключений нет.

## `netPoll(handles, timeoutMs = 0)`

Ждёт готовность listener/socket handles и возвращает массив готовых handles.

```javascript
var ready = netPoll(Array{listener, client}, 100);
```

`timeoutMs`:

- `0` — не ждать;
- положительное значение — ждать указанное число миллисекунд;
- `-1` — ждать без ограничения.

## `netSend(socket, data)`

Отправляет всю строку и возвращает число отправленных байт.

```javascript
netSend(socket, "hello\n");
```

## `netReceive(socket, maxBytes = 4096)`

Читает до `maxBytes` байт. Максимум — 1 MiB.

Результат:

```javascript
Array{"data", "received text"}
Array{"wait"}
Array{"closed"}
```

## `netReceiveLine(socket)`

Читает строку до `\n`. Символы `\n` и завершающий `\r` в результат не входят. Максимальный внутренний буфер строки — 1 MiB.

## `netPort(handle)`

Возвращает локальный порт listener/socket.

## `netClose(handle)`

Закрывает handle. Повторное использование закрытого handle является runtime-ошибкой.

## Event loop

```javascript
var listener = netListen("0.0.0.0", 4040);
var clients = Array{};

while (true) {
    var watched = Array{listener} + clients;
    var ready = netPoll(watched, 100);

    for (var handle in ready) {
        if (handle == listener) {
            var client = netAccept(listener);
            if (client != NULL)
                push(clients, client);
            continue;
        }

        var packet = netReceiveLine(handle);

        if (packet[0] == "data")
            netSend(handle, packet[1] + "\n");
    }
}
```

Сокеты неблокирующие, но `netSend` при заполнении системного буфера может ждать готовности сокета к записи.
