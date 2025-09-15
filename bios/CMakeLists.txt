## makecold
add_executable(makecold EXCLUDE_FROM_ALL makecold.cpp)
target_link_libraries(makecold common)

## coldload.bin
add_custom_command(OUTPUT coldload.bin
    COMMAND sdasrab -o coldload.rel ${CMAKE_CURRENT_SOURCE_DIR}/coldload.s
    COMMAND sdld -i coldload.ihx coldload.rel
    COMMAND sdobjcopy -I ihex -O binary coldload.ihx coldload.obj
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/makecold coldload.obj coldload.bin
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/coldload.s makecold
)
add_custom_target(coldload ALL DEPENDS coldload.bin)

## pilot.bin
add_custom_target(pilot ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pilot.bin)

## install
install(TARGETS makecold DESTINATION ${BIOS_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/coldload.bin pilot.bin DESTINATION ${BIOS_INSTALL_DIR})
