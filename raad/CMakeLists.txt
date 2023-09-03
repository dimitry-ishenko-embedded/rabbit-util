####################
## raad
add_executable(raad
    main.cpp
    rabbit.cpp
    rabbit.hpp
    serial.cpp
    serial.hpp
)
target_compile_definitions(raad
    PRIVATE BIOS_DIR="${BIOS_INSTALL_FULL_DIR}"
    PRIVATE VERSION="${PROJECT_VERSION}"
    PRIVATE ASIO_HAS_IO_URING ASIO_DISABLE_EPOLL
)
target_link_libraries(raad pgm_args uring)
