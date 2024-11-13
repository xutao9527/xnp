# 设置项目名称
project(xnp_mcall LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt5_add_resources(APP_RESOURCES resources/app.qrc)
set(CODE_SOURCES
        src/main.cpp
        resources/app.rc
        ${APP_RESOURCES}
)
add_executable(${PROJECT_NAME} WIN32 ${CODE_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        ICON "resources/img/nim.ico"
)
target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Core Qt5::Qml Qt5::Quick
)

#add_custom_target(copy_${PROJECT_NAME}_resources ALL
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${CMAKE_SOURCE_DIR}/3rdparty/binary-library/win64/runtime" "${CMAKE_CURRENT_BINARY_DIR}"
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${PROJECT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources"
##        COMMAND ${CMAKE_COMMAND} -E copy
##        "${PROJECT_SOURCE_DIR}/resources/config.ini" "${CMAKE_CURRENT_BINARY_DIR}"
#        COMMENT "Copying resources to the build directory"
#        VERBATIM
#)
#add_dependencies(${PROJECT_NAME} copy_${PROJECT_NAME}_resources)