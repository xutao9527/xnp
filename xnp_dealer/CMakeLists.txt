cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(xnp_dealer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED )

set(CODE_SOURCES
        src/main.cpp
        src/ui/RmlQWidget.h
        src/ui/RmlQWidget.cpp
        src/renderer/DbgRenderer.h
        src/renderer/DbgRenderer.cpp
)

#set(RESOURCE_FILES
#        resources.qrc
#)
#qt5_add_resources(RESOURCES ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} ${CODE_SOURCES} resources.rc)

target_link_libraries(${PROJECT_NAME}
        Qt5::Widgets
        xnp-core
)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC src/ui)
target_include_directories(${PROJECT_NAME} PUBLIC src/renderer)

# copy_qt_dll，每次构建时执行
add_custom_target(copy_qt_dll ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/3rdparty/binary-library/win64/qt/" "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Copying Qt DLLs to the build directory"
        VERBATIM
)

# 将 copy_qt_dll 目标与 your_executable 关联
add_dependencies(${PROJECT_NAME} copy_qt_dll)

