cmake_minimum_required(VERSION 3.16.0)

if(NOT MODS_BEING_BUILD STREQUAL "all")
    set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
    set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
endif()

# Start Project
project(glider)

# Include SDK
include("$ENV{HOME}/.minecraft-pi/sdk/lib/minecraft-pi-reborn-client/sdk/sdk.cmake")

# Build the jetpack
add_library(glider SHARED src/glider.cpp)
target_link_libraries(glider mods-headers reborn-patch symbols misc mods-headers media-layer-core)

# Install custom glider textures
install(FILES img/glider_1.png DESTINATION "$ENV{HOME}/.minecraft-pi/overrides/images/armor")
install(FILES img/items.png DESTINATION "$ENV{HOME}/.minecraft-pi/overrides/images/gui")
install(FILES lang/en_US.lang DESTINATION "$ENV{HOME}/.minecraft-pi/overrides/lang/")