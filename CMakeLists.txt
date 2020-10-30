#Autore: Marco Martini 1189321

cmake_minimum_required(VERSION 3.5)

project(Assegnamento2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Charts REQUIRED)
find_package(Threads REQUIRED)

add_executable(Assegnamento2
  main.cpp
  eegplayer.cpp
  eegplayer_b.cpp
  eegplayer.h
  eegplayer.ui
  chart.cpp
  chart.h
  linechart.cpp
  linechart.h
  Player.cpp
  Player.h
  Reader.h
  Reader.cpp
)

target_link_libraries(Assegnamento2 PRIVATE Qt5::Widgets)
target_link_libraries(Assegnamento2 PRIVATE Qt5::Charts)
target_link_libraries(Assegnamento2 PRIVATE ${CMAKE_THREAD_LIBS_INIT} )
