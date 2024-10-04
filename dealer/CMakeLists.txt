project(dealer)

# 搜索wxWidgets库
set(wxWidgets_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/gcc_x64_lib")
find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(RmlUi REQUIRED)
find_package(OpenGL REQUIRED)

file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}")



set(CODE_SOURCES
        src/main.cpp
        src/ui/XnpFrame.h
        src/ui/XnpFrame.cpp

)

add_executable(${PROJECT_NAME} WIN32
        ${CODE_SOURCES}
        resources.rc)

target_include_directories(${PROJECT_NAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        xnp-core
        -static
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC src/ui)