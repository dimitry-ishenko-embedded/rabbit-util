cmake_minimum_required(VERSION 3.16) # focal
project(rabbit-util VERSION 0.3.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

set(BIOS_INSTALL_DIR ${CMAKE_INSTALL_LIBEXECDIR}/${CMAKE_PROJECT_NAME})
set(BIOS_INSTALL_FULL_DIR ${CMAKE_INSTALL_FULL_LIBEXECDIR}/${CMAKE_PROJECT_NAME})

####################
add_subdirectory(common)
add_subdirectory(pgm)

add_subdirectory(bios)
add_subdirectory(raad)
