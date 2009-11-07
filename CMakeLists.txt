
cmake_minimum_required(VERSION 2.6)
project(RemoteWar)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

set(SRC_SHARED
src/Client.cpp
src/ClientContainer.cpp
src/ClientPlayer.cpp
src/ContainerFactory.cpp
src/Game.cpp
src/PlayerFactory.cpp
src/Server.cpp
src/ServerContainer.cpp
src/ServerPlayer.cpp
)
set(SRC_CLIENT
src/main.cpp
)
set(SRC_SERVER
src/servermain.cpp
)

include_directories(include)

add_library(RemoteWarShared SHARED ${SRC_SHARED})
target_link_libraries(RemoteWarShared PeakEngine)

add_executable(RemoteWar ${SRC_CLIENT})
target_link_libraries(RemoteWar RemoteWarShared PeakGraphics PeakPhysics BulletSoftBody BulletDynamics BulletCollision LinearMath)

add_executable(Server ${SRC_SERVER})
target_link_libraries(Server RemoteWarShared PeakGraphics PeakPhysics BulletSoftBody BulletDynamics BulletCollision LinearMath)

set(CMAKE_BUILD_TYPE debug)
