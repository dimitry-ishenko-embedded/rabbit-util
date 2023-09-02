####################
## makecold
add_executable(makecold EXCLUDE_FROM_ALL makecold.cpp)
target_compile_definitions(makecold PRIVATE ASIO_HAS_IO_URING ASIO_DISABLE_EPOLL)
target_link_libraries(makecold uring)

## coldload.bin
add_custom_command(OUTPUT coldload.ihx
    COMMAND sdcc -mr2k --no-std-crt0 --code-loc 0 ${CMAKE_CURRENT_SOURCE_DIR}/coldload.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/coldload.c
)
add_custom_command(OUTPUT coldload.obj
    COMMAND objcopy -I ihex -O binary coldload.ihx coldload.obj
    DEPENDS coldload.ihx
)
add_custom_command(OUTPUT coldload.bin
    COMMAND ./makecold coldload.obj coldload.bin
    DEPENDS coldload.obj makecold
)
add_custom_target(coldload ALL DEPENDS coldload.bin)

## pilot.bin
add_custom_target(pilot ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pilot.bin)

## install
install(TARGETS makecold DESTINATION ${BIOS_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/coldload.bin pilot.bin DESTINATION ${BIOS_INSTALL_DIR})
