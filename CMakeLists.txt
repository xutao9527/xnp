cmake_minimum_required(VERSION 3.15)

# 设置项目信息
project("xnp" VERSION "1.0.0" LANGUAGES "C" "CXX")

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE}")

# 设置安装目录变量(全局)
set_property(GLOBAL PROPERTY XXNP_INSTALL_PATH ${CMAKE_INSTALL_PREFIX})
get_property(XXNP_INSTALL_PATH GLOBAL PROPERTY XXNP_INSTALL_PATH)

# 清空系统默认路径
set(CMAKE_PREFIX_PATH "")
# 设置自定义路径作为首选路径
set(CMAKE_PREFIX_PATH "${XXNP_INSTALL_PATH}")

# 安装第三方库
add_subdirectory(3rdparty)
#find_package(Lua REQUIRED CONFIG)
#message(${LUA_LIBRARY})
#message(${LUA_INCLUDE_DIR})


# 核心
#add_subdirectory(core)

#add_subdirectory(dealer)
