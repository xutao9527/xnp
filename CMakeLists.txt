cmake_minimum_required(VERSION 3.15)

# 设置项目信息
project("xnp" VERSION "1.0.0" LANGUAGES "C" "CXX")

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 设置库安装路径
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE}")
# 清空搜索库路径
set(CMAKE_PREFIX_PATH "")
# 设置搜索库路径
set(CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")

# 安装第三方库
add_subdirectory(3rdparty)
## core
#add_subdirectory(core)
## dealer
#add_subdirectory(dealer)

# xnp_core
add_subdirectory(xnp_core)
# xnp_dealer
add_subdirectory(xnp_dealer)

