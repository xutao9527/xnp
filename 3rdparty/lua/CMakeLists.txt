cmake_minimum_required(VERSION 3.24)
project(lua VERSION 5.4.7 LANGUAGES C CXX)

#================================
# Option variables.
#================================
option(LUA_BUILD_COMPILER "Build Lua compiler" ON)
option(LUA_BUILD_INTERPRETER "Build Lua interpreter" ON)
option(LUA_INSTALL_TARGET "Create install target" OFF)

#================================
# Directory variables.
#================================
set(LUA_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(LUA_SOURCE_ROOT ${LUA_SOURCE_DIR}/lua-5.4.7/src)

#================================
# Add source cmake.
#================================
include(lua_source_definition.cmake)

#message(STATUS ${LUA_LIBRARY_SOURCE})
#message(STATUS ${LUA_LIBRARY_INCLUDE})
#message(STATUS ${LUA_LIBRARY_PUBLIC_INCLUDE})
#message(STATUS ${LUA_COMPILER_SOURCE})
#message(STATUS ${LUA_INTERPRETER_SOURCE})

#================================
# Packages.
#================================
set(LUA_LIBS)
if (UNIX AND NOT APPLE)
    include(CheckLibraryExists)
    check_library_exists(m sin "" HAVE_LIB_M)
    if (HAVE_LIB_M)
        list(APPEND LUA_LIBS m)
    endif()
endif()

#================================
# Lua library.
#================================
source_group("include" FILES ${LUA_LIBRARY_INCLUDE})
source_group("source" FILES ${LUA_LIBRARY_SOURCE})
add_library(lua_lib ${LUA_LIBRARY_INCLUDE} ${LUA_LIBRARY_SOURCE})
target_include_directories(lua_lib PUBLIC
        $<BUILD_INTERFACE:${LUA_SOURCE_ROOT}>
        $<INSTALL_INTERFACE:include>
)
if (UNIX AND NOT APPLE)
    target_link_libraries(lua_lib PUBLIC ${LUA_LIBS})
endif()
target_compile_features(lua_lib PRIVATE cxx_std_20)
set_target_properties(lua_lib PROPERTIES DEBUG_POSTFIX "d")
add_library(lua::lua ALIAS lua_lib)
set_target_properties(lua_lib PROPERTIES FOLDER "lua")

#================================
# Lua compiler.
#================================
if (LUA_BUILD_COMPILER)
    source_group("source" FILES ${LUA_COMPILER_SOURCE})
    add_executable(luac ${LUA_COMPILER_SOURCE})
    target_link_libraries(luac PRIVATE lua_lib)
    set_target_properties(luac PROPERTIES FOLDER "lua")
endif()

#================================
# Lua interpreter
#================================
if (LUA_BUILD_INTERPRETER)
    source_group("source" FILES ${LUA_INTERPRETER_SOURCE})
    add_executable(lua ${LUA_INTERPRETER_SOURCE})
    target_link_libraries(lua PRIVATE lua_lib)
    set_target_properties(lua PROPERTIES FOLDER "lua")
endif()
