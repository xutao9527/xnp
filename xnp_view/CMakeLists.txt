# 设置项目名称
project(xnp_view LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Widgets )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CODE_SOURCES
        src/main.cpp
        src/ui/FcruQWidget.cpp
        src/ui/FcruQWidget.h
        src/ui/BImageWidget.cpp
        src/ui/BImageWidget.h
        src/ui/DImageWidget.cpp
        src/ui/DImageWidget.h
)

add_executable(${PROJECT_NAME} WIN32 ${CODE_SOURCES} resources.rc)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Widgets
)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/3rdparty/binary-library/win64/runtime" "${CMAKE_CURRENT_BINARY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources"
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/resources/config.ini" "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Copying resources to the build directory"
        VERBATIM
)
add_dependencies(${PROJECT_NAME} copy_resources)