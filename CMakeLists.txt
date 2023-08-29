cmake_minimum_required(VERSION 3.1)
project(rabbit-util VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)

set(BIOS_INSTALL_DIR ${CMAKE_INSTALL_LIBEXECDIR}/${CMAKE_PROJECT_NAME})

####################
add_subdirectory(bios)
