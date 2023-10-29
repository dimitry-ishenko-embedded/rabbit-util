####################
add_library(common OBJECT
    file.cpp file.hpp
    message.hpp
    rabbit.hpp
    serial.cpp serial.hpp
    types.cpp types.hpp
)
target_include_directories(common INTERFACE .)
