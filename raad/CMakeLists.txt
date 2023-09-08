####################
add_executable(raad
    main.cpp
    raad.cpp raad.hpp
)
target_compile_definitions(raad
    PRIVATE BIOS_DIR="${BIOS_INSTALL_FULL_DIR}"
    PRIVATE VERSION="${PROJECT_VERSION}"
)
target_link_libraries(raad common pgm_args)

install(TARGETS raad DESTINATION ${CMAKE_INSTALL_BINDIR})
