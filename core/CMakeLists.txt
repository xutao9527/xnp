project(xnp-core)

# 搜索wxWidgets库
set(wxWidgets_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/gcc_x64_lib")

find_package(wxWidgets QUIET COMPONENTS core base)
find_package(RmlUi QUIET)
if(NOT wxWidgets_FOUND)
    message(WARNING "wxWidgets not found, Please install it.")
    return()
endif()

if(NOT RmlUi_FOUND)
    message(WARNING "RmlUi not found, Please install it.")
    return()
endif()


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
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${wxWidgets_LIBRARIES}
        RmlUi::RmlUi
        imm32
        -static
)

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC src/backends)
target_include_directories(${PROJECT_NAME} PUBLIC src/renderer)
target_include_directories(${PROJECT_NAME} PUBLIC src/shell)
target_include_directories(${PROJECT_NAME} PUBLIC src/event)
