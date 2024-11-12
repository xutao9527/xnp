# 设置项目名称
project(xnp_mcall LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Widgets )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CODE_SOURCES
        src/main.cpp

)
add_executable(${PROJECT_NAME} WIN32 ${CODE_SOURCES} resources.rc)

set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        ICON "resources/img/nim.ico"
)
target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Widgets
)

