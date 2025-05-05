# yadro-2025-test-cpp

Данное приложение было написано с использованием компилятора G++ 14.2.0 и системы сборки CMake 3.29.0

# Инструкция по компиляции

Из корня репозитория:

```shell
cmake -S . -B build
# может понадобиться дописать -G "MinGW Makefiles"

# для компиляции приложения
cmake --build build -t task

# для компиляции тестов
cmake --build build -t tests
```

Запуск приложения
```shell
cd build
./task.exe file.txt
```

Запуск тестов
```shell
cd build/test
ctest
```