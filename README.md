# Thread pool (C++ 17)

Разработан тестовый тред пул с возможнотью добавления задач вручную

# Сборка

```bash
sudo install cmake (brew install cmake)

mkdir build && cd build

cmake ..

make
```
# Запуск

Для запуска после создании папки build и перехода в нее необходимо выполнить команду ./thread-pool

# Управление 

Enter + 1 - добавляем задачу 'add_task' в пул
Enter + 2 - добавляем задачу 'many_add_task' в пул
Enter + 3 - добавляем задачу 'slow_task' в пул
Enter + q - выходим, дожидаясь выполнения всех задач
Enter + s - выходим, отменяя задачи, которые возможно отменить

# Архитектура
|
|- /include - thread-pool.hpp
|
|- /src - thread-pool.cpp
|
|-CMakeLists.txt
|-Readme.md

# Авторство

By Nikolaenkov Aleksandr
