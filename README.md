# Thread pool (C++ 17)

Разработан тестовый тред пул с возможнотью добавления задач вручную

## Сборка

```bash
sudo install cmake (brew install cmake)

mkdir build && cd build

cmake ..

make
```
## Запуск

Для запуска после создании папки build и перехода в нее необходимо выполнить команду ./thread-pool

## Управление 

- `1` + Enter - добавляем задачу `add_task` в пул

- `2` + Enter - добавляем задачу `many_add_task` в пул

- `3` + Enter - добавляем задачу `slow_task` в пул

- `q` + Enter - выходим, дожидаясь выполнения всех задач

- `s` + Enter - выходим, отменяя задачи, которые возможно отменить


## Архитектура

```
|
|- /include - thread-pool.hpp
|
|- /src - thread-pool.cpp
|
|-CMakeLists.txt
|-README.md
```
## Авторство

By Nikolaenkov Aleksandr
