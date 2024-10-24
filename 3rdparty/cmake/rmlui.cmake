foreach(FREETYPE_DLL_FILE ${FREETYPE_DLL_FILES})
    file(COPY "${FREETYPE_DLL_FILE}" DESTINATION "${CMAKE_BINARY_DIR}")
endforeach()
# 检查RmlUI是否已经安装
if (EXISTS ${CMAKE_INSTALL_PREFIX}/lib/rmlui.lib)
#    file(COPY "${CMAKE_INSTALL_PREFIX}/bin/" DESTINATION "${CMAKE_BINARY_DIR}")
    message(STATUS "******************** RmlUi is already installed ********************")
else ()
    set(BUILD_SHARED_LIBS ON)           # RMLUI动态库
    set(RMLUI_SVG_PLUGIN OFF)           # RMLUI插件SVG
    set(RMLUI_LUA_BINDINGS OFF)          # RMLUI插件Lua
    set(RMLUI_SAMPLES ON)              # RMLUI库示例程序生成
    set(RMLUI_BACKEND Win32_GL2)         # RMLUI库绘制引擎
    set(RMLUI_IS_ROOT_PROJECT TRUE)     # RMLUI库绘制引擎
    add_subdirectory(RmlUi-6.0)         # RMLUI库生成
endif ()