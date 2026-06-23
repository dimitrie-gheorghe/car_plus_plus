cmake_minimum_required(VERSION 3.26)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# external dependencies with FetchContent
include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE INTERNAL "Override verbose output" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE INTERNAL "Override update strategy" FORCE)
set(SFML_WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.2
        GIT_SHALLOW    1
        GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(SFML)

###############################################################################

# external dependencies with find_package
find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

###############################################################################

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        ${SOURCE_FILES}
        include/SingletonTemplate.h
)

set_compiler_flags(RUN_SANITIZERS ${USE_ASAN} TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

###############################################################################

target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE
        ext
)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE
        SFML::Graphics SFML::Window SFML::System Threads::Threads
)

if(APPLE)
elseif(UNIX)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

###############################################################################

# 1. COPIERE RESURSE PENTRU RULARE LOCALĂ (CLion / Debug)
add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>/assets"
)

add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/config"
        "$<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>/config"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt")
    add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt"
            "$<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>/tastatura.txt"
    )
endif()

###############################################################################

# 2. COPIERE RESURSE PENTRU ARHIVA DE RELEASE
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
install(DIRECTORY assets DESTINATION ${DESTINATION_DIR})
install(DIRECTORY config DESTINATION ${DESTINATION_DIR})

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt")
    install(FILES tastatura.txt DESTINATION ${DESTINATION_DIR})
endif()

if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()