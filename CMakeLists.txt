cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(test_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)

add_executable(main
    main.c
)

# pull in common dependencies
target_link_libraries(main
    pico_stdlib
    pico_multicore
)

# create map/bin/hex file etc.
pico_add_extra_outputs(main)

# add url via pico_set_program_url
example_auto_set_url(main)
