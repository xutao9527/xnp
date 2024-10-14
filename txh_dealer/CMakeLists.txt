cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(tx_dealer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED )

set(CODE_SOURCES
        src/main.cpp
)

set(RESOURCE_FILES
        resources.qrc
)
qt5_add_resources(RESOURCES ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} ${CODE_SOURCES} ${RESOURCES})

target_link_libraries(${PROJECT_NAME}
        Qt5::Widgets
)

# copy_qt_dll，每次构建时执行
add_custom_target(copy_qt_dll ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/3rdparty/binary-library/win64/qt/" "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Copying Qt DLLs to the build directory"
        VERBATIM
)

# 将 copy_qt_dll 目标与 your_executable 关联
add_dependencies(${PROJECT_NAME} copy_qt_dll)

# 创建一个自定义目标 copy_resources，每次构建时执行
#add_custom_target(copy_resources ALL
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${PROJECT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources"
#        COMMENT "Copying resources to the build directory"
#)

# 将 copy_resources 目标与 your_executable 关联
#add_dependencies(${PROJECT_NAME} copy_resources)

# 添加图标资源到可执行文件 (仅在 Windows 平台上)
#if(WIN32)
#    set(APP_ICON_RESOURCE "${PROJECT_SOURCE_DIR}/resources.rc")
#    target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_RESOURCE})
#endif()