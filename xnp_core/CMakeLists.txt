project(xnp-core)

find_package(RmlUi QUIET)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenGL REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SHELL_SOURCES
        src/shell/Shell.cpp
        src/shell/Shell.h
        src/shell/ShellFileInterface.cpp
        src/shell/ShellFileInterface.h
)

set(BACKENDS_SOURCES
        src/backends/RmlUi_Platform_Win32.h
        src/backends/RmlUi_Platform_Win32.cpp
        src/backends/RmlUi_Renderer_GL2.cpp
        src/backends/RmlUi_Renderer_GL2.h
)

set(CODE_SOURCES
        src/rml/XnpWinGLWidget.cpp
        src/rml/XnpWinGLWidget.h
)

add_library(${PROJECT_NAME}
        ${SHELL_SOURCES}
        ${BACKENDS_SOURCES}
        ${CODE_SOURCES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_include_directories(${PROJECT_NAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Widgets
        OpenGL::GL
        RmlUi::RmlUi
        imm32
)

target_include_directories(${PROJECT_NAME} PUBLIC src/rml)
target_include_directories(${PROJECT_NAME} PUBLIC src/backends)
target_include_directories(${PROJECT_NAME} PUBLIC src/renderer)
target_include_directories(${PROJECT_NAME} PUBLIC src/shell)

