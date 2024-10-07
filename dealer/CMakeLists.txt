project(dealer)

file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}")

set(CODE_SOURCES
        src/main.cpp
        src/ui/XnpFrame.h
        src/ui/XnpFrame.cpp
        src/renderer/DbgRenderer.h
        src/renderer/DbgRenderer.cpp
)

add_executable(${PROJECT_NAME} WIN32
        ${CODE_SOURCES}
        resources.rc)

target_include_directories(${PROJECT_NAME} PUBLIC ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        xnp-core
        -static
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_include_directories(${PROJECT_NAME} PUBLIC src/ui)
target_include_directories(${PROJECT_NAME} PUBLIC src/renderer)