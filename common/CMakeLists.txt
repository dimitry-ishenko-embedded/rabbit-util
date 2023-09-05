####################
add_library(common OBJECT
    file.hpp
    message.hpp
    serial.cpp serial.hpp
)
target_include_directories(common INTERFACE .)
target_compile_definitions(common INTERFACE ASIO_HAS_IO_URING ASIO_DISABLE_EPOLL)
