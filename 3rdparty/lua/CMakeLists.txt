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
message(STATUS ${LUA_LIBRARY_SOURCE})
message(STATUS ${LUA_LIBRARY_INCLUDE})
message(STATUS ${LUA_LIBRARY_PUBLIC_INCLUDE})
#================================
# Lua library.
#================================
source_group("include" FILES ${LUA_LIBRARY_INCLUDE})
source_group("source" FILES ${LUA_LIBRARY_SOURCE})
add_library(lua54 ${LUA_LIBRARY_INCLUDE} ${LUA_LIBRARY_SOURCE})
target_include_directories(lua54 PUBLIC
        $<BUILD_INTERFACE:${LUA_SOURCE_ROOT}>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(lua54 PRIVATE cxx_std_20)
set_target_properties(lua54 PROPERTIES DEBUG_POSTFIX "d")
add_library(lua::lua ALIAS lua54)
set_target_properties(lua54 PROPERTIES FOLDER "lua")
#================================
# Lua install
#================================
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
# 安装目标库
install(TARGETS lua54
        EXPORT LuaTargets                           # 用于导出目标
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}    # 安装共享库到 lib 目录
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}    # 安装静态库到 lib 目录
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}    # 安装可执行文件到 bin 目录（Windows 下的 DLL）
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}  # 安装头文件到 include 目录
)
# 安装头文件
install(FILES ${LUA_LIBRARY_PUBLIC_INCLUDE}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lua
)
# 安装导出文件
install(EXPORT LuaTargets
        FILE LuaTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lua
        NAMESPACE Lua::
)
# 生成 CMake 包配置文件
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/LuaConfigVersion.cmake"
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/LuaConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/LuaConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lua
)
# 安装配置文件
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/LuaConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/LuaConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lua
)

