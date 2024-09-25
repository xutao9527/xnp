# 设置安装目录
get_property(XXNP_INSTALL_PATH GLOBAL PROPERTY XXNP_INSTALL_PATH)

# 检查wxWidgets-3.2.6是否已经安装
if (EXISTS ${XXNP_INSTALL_PATH}/lib/gcc_x64_lib/libwxbase32ud.a OR EXISTS "${XXNP_INSTALL_PATH}/lib/gcc_x64_lib/libwxbase32u.a")
    message(STATUS "******************** wxWidgets is already installed ********************")
#    set(wxWidgets_ROOT_DIR "${XXNP_INSTALL_PATH}")
#    set(wxWidgets_LIB_DIR "${XXNP_INSTALL_PATH}/lib/gcc_x64_lib")
#    find_package(wxWidgets REQUIRED)
else ()
#    set(wxBUILD_SAMPLES All)
    set(wxBUILD_SHARED OFF)
    set(wxBUILD_MONOLITHIC OFF)
    set(wxBUILD_USE_STATIC_RUNTIME ON)
    add_subdirectory(wxWidgets-3.2.6)
endif ()

# 检查lua-5.4.7是否已经安装
if (EXISTS ${XXNP_INSTALL_PATH}/lib/gcc_x64_lib/liblua_staticd.a OR EXISTS "${XXNP_INSTALL_PATH}/lib/gcc_x64_lib/liblua_static.a")
    message(STATUS "******************** lua is already installed ********************")
else ()
    add_subdirectory(Lua)
endif ()