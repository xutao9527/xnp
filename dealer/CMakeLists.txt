# 获得安装目录
get_property(XXNP_INSTALL_PATH GLOBAL PROPERTY XXNP_INSTALL_PATH)

project(dealer)

# 判断
if (NOT (EXISTS "${XXNP_INSTALL_PATH}/lib/gcc_x64_lib/libwxbase32ud.a" OR EXISTS "${XXNP_INSTALL_PATH}/lib/gcc_x64_lib/libwxbase32u.a"))
    message(STATUS "******************** wxWidgets not install ********************")
    return()
endif ()

set(wxWidgets_ROOT_DIR "${XXNP_INSTALL_PATH}")
set(wxWidgets_LIB_DIR "${XXNP_INSTALL_PATH}/lib/gcc_x64_lib")
find_package(wxWidgets REQUIRED COMPONENTS webview xrc xml html core base)


file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


set(CODE_SOURCES
        src/main.cpp
        src/wxSkinXFrame.h
        src/wxSkinXFrame.cpp
)


add_executable(${PROJECT_NAME} WIN32 ${CODE_SOURCES} resources.rc)
target_include_directories(${PROJECT_NAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${wxWidgets_LIBRARIES}
        -static
)