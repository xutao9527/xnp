# 获得安装目录
get_property(XXNP_INSTALL_PATH GLOBAL PROPERTY XXNP_INSTALL_PATH)

project(xnp-core C)

add_executable(lua-test src/lua/main.c)
target_link_libraries(lua-test lua_static)