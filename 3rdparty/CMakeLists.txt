# 设置安装目录
get_property(XXNP_INSTALL_PATH GLOBAL PROPERTY XXNP_INSTALL_PATH)

# 检查wxWidgets-3.2.6是否已经安装
if (EXISTS ${XXNP_INSTALL_PATH}/lib/gcc_x64_lib/libwxbase32ud.a OR EXISTS "${XXNP_INSTALL_PATH}/lib/gcc_x64_lib/libwxbase32u.a")
    message(STATUS "******************** wxWidgets is already installed ********************")
#    set(wxWidgets_ROOT_DIR "${XXNP_INSTALL_PATH}")
#    set(wxWidgets_LIB_DIR "${XXNP_INSTALL_PATH}/lib/gcc_x64_lib")
#    find_package(wxWidgets REQUIRED)
#    message(${wxWidgets_LIBRARIES})
#    message(${wxWidgets_INCLUDE_DIRS})
else ()
#    set(wxBUILD_SAMPLES All)
    set(wxBUILD_SHARED OFF)
    set(wxBUILD_MONOLITHIC OFF)
    set(wxBUILD_USE_STATIC_RUNTIME ON)
    add_subdirectory(wxWidgets-3.2.6)
endif ()

add_subdirectory(lua)