project(xnp-core)

# 搜索wxWidgets库
set(wxWidgets_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/gcc_x64_lib")
find_package(wxWidgets REQUIRED COMPONENTS core base png)
find_package(RmlUi REQUIRED)
find_package(OpenGL REQUIRED)

set(SHELL_SOURCES
        src/shell/Shell.cpp
        src/shell/Shell.h
        src/shell/ShellFileInterface.cpp
        src/shell/ShellFileInterface.h
)

set(BACKENDS_SOURCES
        src/backends/RmlUi_Platform_Win32.h
        src/backends/RmlUi_Platform_Win32.cpp
        src/backends/RmlUi_Renderer_VK.h
        src/backends/RmlUi_Renderer_VK.cpp
        src/backends/RmlUi_Include_Vulkan.h
        src/backends/RmlUi_Include_Windows.h
        src/backends/RmlUi_Backend.h
)

set(CODE_SOURCES
        src/renderer/XnpWin32VKContext.h
        src/renderer/XnpWin32VKContext.cpp
        src/event/XnpEventHandler.h
        src/event/XnpEventHandler.cpp
)

add_library(${PROJECT_NAME}
        ${SHELL_SOURCES}
        ${BACKENDS_SOURCES}
        ${CODE_SOURCES}
)
target_include_directories(${PROJECT_NAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
        RmlUi::RmlUi
        ${wxWidgets_LIBRARIES}
        imm32
        -static
)

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC src/backends)
target_include_directories(${PROJECT_NAME} PUBLIC src/renderer)
target_include_directories(${PROJECT_NAME} PUBLIC src/shell)
target_include_directories(${PROJECT_NAME} PUBLIC src/event)
