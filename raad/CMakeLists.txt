####################
## raad
include_directories(..)

add_executable(raad
    main.cpp
    serial.cpp
    serial.hpp
)
target_compile_definitions(raad
    PRIVATE BIOS_DIR="${BIOS_INSTALL_FULL_DIR}"
    PRIVATE VERSION="${PROJECT_VERSION}"
)
