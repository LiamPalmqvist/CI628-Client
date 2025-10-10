
cmake_minimum_required(VERSION 3.6)

# the project name is Client
project(Client CXX)

set(CMAKE_CXX_STANDARD 11)

# let FindSDL2.cmake figure out where it is on Mac/Linux (though we're overriding with locals)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Find the standard SDL2 libraries installed on the system
find_package(SDL2 REQUIRED)

# include SDL header files
include_directories(${SDL2_INCLUDE_DIRS})

# load user source and header files
file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES}
        main.cpp)

# make assets directory in build
#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)

# copy game assets into build/assets directory
#file(COPY
#        "assets/assetName.png"
#        DESTINATION ${CMAKE_BINARY_DIR}/assets)

# SDL2MAIN_LIBRARY is needed for Windows specific main function.
target_link_libraries(${PROJECT_NAME})



