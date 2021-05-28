add_executable(temp_tracker
        temp_tracker.c
        si7021_picopi.c
        si7021_picopi.h
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(temp_tracker pico_stdlib hardware_i2c )

# create map/bin/hex file etc.
pico_add_extra_outputs(temp_tracker)

# enable serial communication over usb
pico_enable_stdio_usb(temp_tracker 1)
pico_enable_stdio_uart(temp_tracker 0)

# add url via pico_set_program_url
example_auto_set_url(temp_tracker)
