add_executable(blink_led
        blink_led.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(blink_led pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(blink_led)

# enable serial communication over usb
pico_enable_stdio_usb(blink_led 1)
pico_enable_stdio_uart(blink_led 0)

# add url via pico_set_program_url
example_auto_set_url(blink_led)
