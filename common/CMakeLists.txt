####################
add_library(common OBJECT
    file.cpp file.hpp
    message.hpp
    rabbit.hpp
    serial.cpp serial.hpp
    types.hpp
)
target_include_directories(common PUBLIC .)
target_compile_definitions(common PUBLIC ASIO_HAS_IO_URING ASIO_DISABLE_EPOLL)
