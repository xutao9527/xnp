# 检查wxWidgets-3.2.6是否已经安装
if (EXISTS ${CMAKE_INSTALL_PREFIX}/lib/gcc_x64_lib/libwxbase32ud.a OR EXISTS "${CMAKE_INSTALL_PREFIX}/lib/gcc_x64_lib/libwxbase32u.a")
    message(STATUS "******************** wxWidgets is already installed ********************")
    set(wxWidgets_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/gcc_x64_lib")
    find_package(wxWidgets REQUIRED)
else ()
    #set(wxBUILD_SAMPLES All)
    set(wxBUILD_SHARED OFF)
    set(wxBUILD_MONOLITHIC OFF)
    set(wxBUILD_USE_STATIC_RUNTIME ON)
    add_subdirectory(wxWidgets-3.2.6)
endif ()