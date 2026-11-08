cmake_minimum_required(VERSION 3.16)

project(Actividad1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Actividad1 main.cpp
    Particula.h
    Obstaculo.h
    Simulacion.h
    Simulacion.cpp)

include(GNUInstallDirs)
install(TARGETS Actividad1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
