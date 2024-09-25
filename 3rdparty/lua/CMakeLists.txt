#================================
# Project setup.
#================================
cmake_minimum_required(VERSION 3.24)
project(lua VERSION 5.4.7 LANGUAGES C CXX)
#================================
# Directory variables.
#================================
set(LUA_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(LUA_SOURCE_ROOT ${LUA_SOURCE_DIR}/lua-5.4.7/src)
#================================
# Add source cmake.
#================================
include(cmake/lua_code_def.cmake)
#================================
# Lua library.
#================================
add_library(lua ${LUA_LIBRARY_INCLUDE} ${LUA_LIBRARY_SOURCE})
target_include_directories(lua PUBLIC
        $<BUILD_INTERFACE:${LUA_SOURCE_ROOT}>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(lua PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(lua PROPERTIES VERSION "${PACKAGE_VERSION}")
add_library(lua::lua ALIAS lua)
#================================
# lua install
#================================
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# 安装目标库
install(TARGETS lua
        EXPORT luaTargets                                       # 用于导出目标
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}             # 安装共享库到 lib 目录
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}             # 安装静态库到 lib 目录
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}             # 安装可执行文件到 bin 目录（Windows 下的 DLL）
#        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}        # 安装头文件到 include 目录
)
# 安装头文件
install(FILES ${LUA_LIBRARY_PUBLIC_INCLUDE}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lua
)
# 安装导出文件
install(EXPORT luaTargets
        FILE luaTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lua
        NAMESPACE lua::
)
# 生成 CMake 包配置文件
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/luaConfigVersion.cmake"
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY SameMajorVersion
)

# 配置文件
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/luaConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/luaConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lua
)

# 安装配置文件
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/luaConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/luaConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lua
)

