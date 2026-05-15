# impulse kvadraOS Desktop (команда 4) тестовое задание

## Начальные требования

- CMake 3.22+
- Make или другая билд система, поддерживаемая CMake
- C++17 компилятор (тестировалось на gcc 13.3.0)

## Сборка

Из корневой директории проекта:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Использование

```bash
./build/media_finder -h
```

Результат

```markdown
Small (media)file poller server

media_finder [OPTIONS]

OPTIONS:
-h, --help Print this help message and exit
-i, --interval UINT Set polling interval in seconds (defaults to 30)
-p, --path TEXT:DIR Set directory to monitor (defaults to $HOME)
```

Пример запуска

```bash
./build/media_finder -p ~/Documents/ -i 4
```

Ожидаемый вывод:
```bash
starting with directory = "/home/<user>/Documents/", interval = 4 seconds
```

## Описание

Выполнено второе доп. задание (с серверной частью). При реализации использованы библиотеки cpp-httplib, cli11, nlohmann_json.
Так как в задании было написано, что надо именно "обходить" директорию, было принято решение не использовать inotify.

Программа реализована на C++17. Интервал задается в секундах.
