cmake_minimum_required(VERSION 3.24)
project(lua VERSION 5.4.7 LANGUAGES CXX C)

#================================
# Directory variables.
#================================
set(LUA_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(LUA_SOURCE_ROOT ${LUA_SOURCE_DIR}/src)
#================================
# Add include.
#================================
include(cmake/lua_code_def.cmake)
#================================
# Lua library.
#================================
add_library(lua_include INTERFACE)
target_include_directories(lua_include INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
add_library(lua_static ${LUA_LIBRARY_INCLUDE} ${LUA_LIBRARY_SOURCE})
target_link_libraries(lua_static PUBLIC lua_include)
set_target_properties(lua_static PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(lua_static PROPERTIES VERSION "${PACKAGE_VERSION}")
#================================
# Lua interpreter
#================================
add_executable(lua ${LUA_INTERPRETER_SOURCE})
target_link_libraries(lua PRIVATE lua_static)
#================================
# Lua compiler.
#================================
add_executable(luac ${LUA_COMPILER_SOURCE})
target_link_libraries(luac PRIVATE lua_static)


