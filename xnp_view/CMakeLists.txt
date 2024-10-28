# 设置项目名称
project(xnp_view LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CODE_SOURCES
        src/main.cpp
)

add_executable(${PROJECT_NAME} ${CODE_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Widgets
)