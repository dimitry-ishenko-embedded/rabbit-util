add_library(pgm_args INTERFACE args.hpp args.ipp)
target_include_directories(pgm_args INTERFACE .)
add_library(pgm::args ALIAS pgm_args)
