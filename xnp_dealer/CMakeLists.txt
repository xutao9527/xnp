cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(xnp_dealer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
set_target_properties(${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
target_link_libraries(${PROJECT_NAME}
        xnp-core
)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC src/ui)
target_include_directories(${PROJECT_NAME} PUBLIC src/renderer)

add_custom_target(copy_dependencies ALL
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${CMAKE_INSTALL_PREFIX}/bin/" "${CMAKE_CURRENT_BINARY_DIR}"
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${CMAKE_SOURCE_DIR}/3rdparty/binary-library/win64/qt/" "${CMAKE_CURRENT_BINARY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources/font" "${CMAKE_CURRENT_BINARY_DIR}/resources/font"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources/layout" "${CMAKE_CURRENT_BINARY_DIR}/resources/layout"
        COMMENT "Copying Qt DLLs to the build directory"
        VERBATIM
)
add_dependencies(${PROJECT_NAME} copy_dependencies)

