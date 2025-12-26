cmake_minimum_required(VERSION 3.14)
project(ScrumBoard VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    # Настройки для macOS
    set(CMAKE_MACOSX_RPATH ON)
endif()

include(FetchContent)

# FTXUI 
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# nlohmann/json 
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Исходные файлы проекта
set(SOURCES
    src/main.cpp
    src/models/task.cpp
    src/models/developer.cpp
    src/models/scrum_board.cpp
    src/persistence/json_serializer.cpp
    src/ui/app.cpp
    src/ui/board_view.cpp
    src/ui/dialogs.cpp
)

# Заголовочные файлы
set(HEADERS
    src/models/task.hpp
    src/models/developer.hpp
    src/models/scrum_board.hpp
    src/persistence/json_serializer.hpp
    src/ui/app.hpp
    src/ui/board_view.hpp
    src/ui/dialogs.hpp
)

# Основной исполняемый файл
add_executable(scrum_board ${SOURCES} ${HEADERS})
target_include_directories(scrum_board PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(scrum_board PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
    nlohmann_json::nlohmann_json
)

# GoogleTest 
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(scrum_lib STATIC
    src/models/task.cpp
    src/models/developer.cpp
    src/models/scrum_board.cpp
    src/persistence/json_serializer.cpp
)
target_include_directories(scrum_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(scrum_lib PUBLIC nlohmann_json::nlohmann_json)

# Исполняемый файл для тестов
add_executable(scrum_tests
    tests/test_task.cpp
    tests/test_developer.cpp
    tests/test_scrum_board.cpp
    tests/test_serializer.cpp
)
target_link_libraries(scrum_tests PRIVATE
    scrum_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(scrum_tests)