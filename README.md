# yadro-2025-test-cpp

������ ���������� ���� �������� � �������������� ����������� G++ 14.2.0 � ������� ������ CMake 3.29.0

# ���������� �� ����������

�� ����� �����������:

```shell
cmake -S . -B build
# ����� ������������ �������� -G "MinGW Makefiles" (���� ������������ MinGW)

# ��� ���������� ����������
cmake --build build -t task

# ��� ���������� ������
cmake --build build -t tests
```

������ ����������

```shell
cd build
./task.exe file.txt
```

������ ������

```shell
cd build/test
ctest
```