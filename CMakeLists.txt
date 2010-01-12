
cmake_minimum_required(VERSION 2.6)
project(Shooter)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

set(SRC_SHARED
src/container/ClientContainer.cpp
src/container/ContainerFactory.cpp
src/container/ServerContainer.cpp
src/player/ClientPlayer.cpp
src/player/PlayerFactory.cpp
src/player/ServerPlayer.cpp
src/star/ClientStar.cpp
src/star/StarFactory.cpp
src/star/ServerStar.cpp
src/Client.cpp
src/Game.cpp
src/HUD.cpp
src/MainMenu.cpp
src/Server.cpp
)
set(SRC_CLIENT
src/main.cpp
)
set(SRC_SERVER
src/servermain.cpp
)

include_directories(include)

add_library(ShooterShared SHARED ${SRC_SHARED})
target_link_libraries(ShooterShared PeakEngine)

add_executable(Shooter ${SRC_CLIENT})
target_link_libraries(Shooter ShooterShared PeakGraphics PeakPhysics BulletSoftBody BulletDynamics BulletCollision LinearMath)

add_executable(Server ${SRC_SERVER})
target_link_libraries(Server ShooterShared PeakGraphics PeakPhysics BulletSoftBody BulletDynamics BulletCollision LinearMath)

set(CMAKE_BUILD_TYPE debug)
