/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * I2C requirements:
 * i2c_set_slave_mode function to determine master or slave
 * reserved_addr to ensure that the reserved addresses aren't being used
 * 
 * 
 * 
 * 
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/i2c.h"
#include <pico/binary_info.h>
#include "si7021_picopi.h"


#define SECONDS_IN_A_YEAR (60 * 60 * 24 * 365 * 10000)
#define MINI(A,B) ((A) <= (B) ? (A) : (B))
#define TEMP_ADDR (0x40) //Temperature and Humidity sensor - SI7021

bool reserved_addr(uint8_t addr) {
    /**
     * Function to return true if the address is reserved, thus don't send
     * a message unless this function does not return true.
     */
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

struct Pico_Si7021 sensor_i2c;
uint16_t data_buffer;
float temp_float;
uint8_t temp_out;

int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
#ifndef TEMP_ADDR
#warning no temperature i2c address set
#else
    // printf("Author - Spencer Walker-Fooks\n");
    // sleep_ms(10000);
    stdio_init_all();
    sleep_ms(2000);
    si7021_init(&sensor_i2c, i2c_default);
    // printf("\nRevision: %d", sensor_i2c->revision);

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        temp_float = si7021_read_temperature(&sensor_i2c);

        printf("\nTemperature: %.2f", temp_float);
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
        // test = test+test;
        // printf("Test value: %d\n", *ptr);
    }
#endif
#endif
}
