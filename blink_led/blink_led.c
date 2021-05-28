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

// // uint8_t *p;
// uint8_t b = 1;
// uint8_t a = 40;
// uint8_t *p = &a;
// uint test = 3;
// // int least = *p;
// // least++;

// int *ptr;

// *ptr = (int *)0x67a9;

// int square(volatile int *ptr)
// {
//     return *ptr * *ptr;
// }


// *ptr = 0xaa55;

void setup_i2c(){
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

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
    setup_i2c(); 

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        printf("LED ON!\n");
        sleep_ms(5000);
        gpio_put(LED_PIN, 0);
        printf("LED OFF!\n");
        sleep_ms(5000);
        // test = test+test;
        // printf("Test value: %d\n", *ptr);
    }
#endif
#endif
}
