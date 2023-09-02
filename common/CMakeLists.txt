####################
add_library(common INTERFACE file.hpp message.hpp)
target_include_directories(common INTERFACE .)
target_compile_definitions(common INTERFACE ASIO_HAS_IO_URING ASIO_DISABLE_EPOLL)
