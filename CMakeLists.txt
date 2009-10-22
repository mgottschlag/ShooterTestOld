
cmake_minimum_required(VERSION 2.6)
project(RemoteWar)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

set(SRC
src/Client.cpp
src/ClientPlayer.cpp
src/Game.cpp
src/main.cpp
src/PlayerFactory.cpp
src/Server.cpp
src/ServerPlayer.cpp
)

include_directories(include)

add_executable(RemoteWar ${SRC})
target_link_libraries(RemoteWar PeakEngine PeakGraphics)

set(CMAKE_BUILD_TYPE debug)
