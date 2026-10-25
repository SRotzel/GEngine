cmake_minimum_required(VERSION 3.20)
project(GEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB MODULES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*"
)

foreach(MODULE_DIR ${MODULES})
    if(IS_DIRECTORY "${MODULE_DIR}")
        get_filename_component(MODULE_NAME "${MODULE_DIR}" NAME)

        file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
            "${MODULE_DIR}/Private/*.cpp"
        )

        add_library(${MODULE_NAME} ${SOURCES})

        target_include_directories(${MODULE_NAME}
            PUBLIC "${MODULE_DIR}/Public"
        )
    endif()
endforeach()

target_link_libraries(AudioEngine
    PUBLIC Core
)
target_link_libraries(AudioEngine
    PRIVATE ThirdParty
)

target_link_libraries(Engine
    PUBLIC Core
)

add_executable(Editor Editor/Editor.h)
target_link_libraries(Editor
    PRIVATE Engine
)