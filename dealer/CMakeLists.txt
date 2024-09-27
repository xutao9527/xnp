project(dealer)


# 搜索wxWidgets库
set(wxWidgets_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/gcc_x64_lib")
find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(RmlUi REQUIRED)
find_package(OpenGL REQUIRED)

file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

set(SHELL_SOURCES
        src/shell/Shell.cpp
        src/shell/Shell.h
        src/shell/ShellFileInterface.cpp
        src/shell/ShellFileInterface.h
)

set(CODE_SOURCES
        src/main.cpp
        src/ui/XnpFrame.h
        src/ui/XnpFrame.cpp
)

add_executable(${PROJECT_NAME}
        ${SHELL_SOURCES}
        ${CODE_SOURCES}
        resources.rc)
target_include_directories(${PROJECT_NAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${wxWidgets_LIBRARIES}
        RmlUi::RmlUi
        OpenGL::GL
        -static
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)