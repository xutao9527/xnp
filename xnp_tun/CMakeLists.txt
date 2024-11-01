# 设置项目名称
project(xnp_tun)

set(CODE_SOURCES
        src/main.cpp
)

add_executable(${PROJECT_NAME} ${CODE_SOURCES})
target_link_libraries(xnp_tun PRIVATE Ws2_32.lib Iphlpapi.lib)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/3rdparty/wintun/include")
add_custom_target(copy_${PROJECT_NAME}_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/3rdparty/wintun/bin/amd64/wintun.dll" "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Copying resources to the build directory"
        VERBATIM
)
add_dependencies(${PROJECT_NAME} copy_${PROJECT_NAME}_resources)
