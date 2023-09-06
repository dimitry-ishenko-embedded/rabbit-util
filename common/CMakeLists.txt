####################
add_library(common OBJECT
    file.cpp file.hpp
    message.hpp
    rabbit.hpp
    serial.cpp serial.hpp
    types.cpp types.hpp
)
target_compile_definitions(common PUBLIC ASIO_HAS_IO_URING ASIO_DISABLE_EPOLL)
target_include_directories(common INTERFACE .)
target_link_libraries(common INTERFACE uring)
