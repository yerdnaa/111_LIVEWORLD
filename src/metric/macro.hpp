#include <concepts>
#include <source_location>
#include <string>
#include <stdexcept>


#pragma once


//* Числовой концепт
template <typename T>
concept is_numeric = std::integral<T> || std::floating_point<T>;


//* Удаление конструктора копирования
#define DELETE_COPY_CONSTR(NAME)\
    NAME(const NAME &) = delete;\
    NAME(NAME &&) = delete;\
    NAME &operator=(const NAME &) = delete;\
    NAME &operator=(NAME &&) = delete;


//* Инициализация ошибки
inline void INIT_ERROR(const std::source_location & loc, const std::string & message) {
    throw std::runtime_error(
        "\n\t" + message + " at " +
        std::string(loc.file_name()) + " :: line " +
        std::to_string(loc.line()) + " inside `" +
        loc.function_name() + "`"
    );
}


//* Перегрузка сразу для многих типов
#define OVERLOAD_ALL_TYPES \
    OVERLOAD(int) \
    OVERLOAD(float) \
    OVERLOAD(double)
