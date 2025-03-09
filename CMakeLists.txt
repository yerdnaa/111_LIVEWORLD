
option(BUILD_TESTS "Включить сборку тестов" OFF)
set(PROJECT_NAME_CUSTOM "Proj" CACHE STRING "Название проекта")

cmake_minimum_required(VERSION 3.12)
project(${PROJECT_NAME_CUSTOM})


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Определяем переменные для путей к библиотекам
set(VULKAN_SDK "C:/VulkanSDK/1.3.296.0")


# Находим библиотеку Vulkan
find_library(VULKAN_LIBRARY NAMES vulkan-1 PATHS ${VULKAN_SDK}/Lib NO_DEFAULT_PATH REQUIRED)

# Устанавливаем флаги компиляции
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Флаги для оптимизации (-O3) и отладочной информации (-g)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
    # Предупреждения и дополнительные флаги
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} src/main.cpp)

# Конфигурация тестов:
if (BUILD_TESTS)
    add_executable(${PROJECT_NAME}_tests src/tests/run_tests.cpp)
endif()

# Указываем путь к заголовочным файлам SDL2
target_include_directories(${PROJECT_NAME} PRIVATE ${VULKAN_SDK}/Include)

# Устанавливаем пути к библиотекам SDL2
link_directories(${VULKAN_SDK}/Lib)

# Добавляем библиотеку SDL2main
find_library(SDL2MAIN_LIBRARY NAMES sdl2main PATHS ${VULKAN_SDK}/Lib NO_DEFAULT_PATH REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2MAIN_LIBRARY})

# Добавляем основную библиотеку SDL2
find_library(SDL2_LIBRARY NAMES sdl2 PATHS ${VULKAN_SDK}/Lib NO_DEFAULT_PATH REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY})

# Добавляем основную библиотеку Vulkan
target_link_libraries(${PROJECT_NAME} PRIVATE ${VULKAN_LIBRARY})


# Генерация сборки:
# cmake -G "MinGW Makefiles" -B build -S .

# Получение исполняемого файла:
# cd build
# mingw32-make