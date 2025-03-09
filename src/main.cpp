#include <iostream>
#include <concepts>

template <std::integral T>
T fun(T arg) {
    return arg;
}

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, ".UTF8");

    std::wcout << L"Привет, мир " << fun(5);
}

