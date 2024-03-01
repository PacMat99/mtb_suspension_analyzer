add_library(ssd1306_display
    ssd1306_display.h
    ssd1306_display.c
    )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(ssd1306_display
        hardware_i2c
        )

#target_link_libraries(ssd1306_i2c pico_stdlib hardware_i2c)

target_include_directories(ssd1306_display PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
