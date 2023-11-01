cmake_minimum_required(VERSION 3.1)
project(rabbit-util VERSION 0.3.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

set(BIOS_INSTALL_DIR ${CMAKE_INSTALL_LIBEXECDIR}/${CMAKE_PROJECT_NAME})
set(BIOS_INSTALL_FULL_DIR ${CMAKE_INSTALL_FULL_LIBEXECDIR}/${CMAKE_PROJECT_NAME})

####################
add_subdirectory(common)

add_library(pgm_args INTERFACE pgm/args.hpp pgm/args.ipp)
target_include_directories(pgm_args INTERFACE .)

add_subdirectory(bios)
add_subdirectory(raad)
