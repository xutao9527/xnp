project(dealer)


# 搜索wxWidgets库
set(wxWidgets_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/gcc_x64_lib")
find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(RmlUi REQUIRED)
find_package(OpenGL REQUIRED)

file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}")

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
        src/backends/RmlUi_Backend_Win32_VK.cpp
        src/backends/RmlUi_Backend.h
)

set(CODE_SOURCES
        src/main.cpp
        src/uiw/XnpFrame.h
        src/uiw/XnpFrame.cpp
        src/uir/XnpRWindow.h
        src/uir/XnpRWindow.cpp
)

add_executable(${PROJECT_NAME}
        ${SHELL_SOURCES}
        ${BACKENDS_SOURCES}
        ${CODE_SOURCES}
        resources.rc)
target_include_directories(${PROJECT_NAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${wxWidgets_LIBRARIES}
        RmlUi::RmlUi
        OpenGL::GL
        imm32
        -static
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC src/uir)
target_include_directories(${PROJECT_NAME} PUBLIC src/uiw)
target_include_directories(${PROJECT_NAME} PUBLIC src/shell)
target_include_directories(${PROJECT_NAME} PUBLIC src/backends)