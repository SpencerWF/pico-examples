/**Library to communicate with the si7021 temperature and humidity sensor,
 * designed after the adafruit arduino library.
 * 
 */

// Includes
#include <stdio.h>
#include "pico/stdlib.h"
#include "si7021_picopi.h"
#include "hardware/i2c.h"
#include <pico/binary_info.h>

uint8_t si7021_init(struct Pico_Si7021 *self, i2c_inst_t* i2c_i) {
    i2c_init(i2c_i, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    self->inst = i2c_i;
    self->i2c_addr = SI7021_DEFAULT_ADDRESS;
    // self->sernum_a = 0;
    // self->sernum_b = 0;
    // si7021_read_revision(self);
#ifdef DEBUG
    printf("\nI2C Bus Initialized For Si7021\nAddress:%d", self->i2c_addr);
#endif
}

uint16_t si7021_read_humidity(struct Pico_Si7021 *self) {
    static uint8_t cmd_data = SI7021_MEASRH_HOLD_CMD;
    uint16_t data_buffer=0;
    float data_float=0;
    int err = 0;

    err = i2c_write_blocking(self->inst, self->i2c_addr, &cmd_data, 1, true);

    if(err!=PICO_ERROR_GENERIC) {
        printf("\nReading from Temp Sensor");

        i2c_read_blocking(i2c_default, self->i2c_addr, &(self->data_out[0]), 2, false);

        data_buffer = self->data_out[1] | (self->data_out[0] << 8);
        data_float = ((125*data_buffer)/65536)-6;
    } else {
        printf("\nReturn: %d", err);
    }
    return data_float;
}

float si7021_read_temperature(struct Pico_Si7021 *self) {
    static uint8_t cmd_data = SI7021_MEASTEMP_NOHOLD_CMD;
    uint8_t data_out[2];
    uint16_t data_buffer=0;
    float data_float = 0;

    int err = 0;

#ifdef DEBUG
    printf("\nWriting to Temp Sensor: 0x%x", self->i2c_addr);
    printf("\nNo Hold CMD: 0x%x", cmd_data);
#endif
    err = i2c_write_blocking(self->inst, self->i2c_addr, &cmd_data, 1, true);

    if(err!=PICO_ERROR_GENERIC) {
        // Delay to allow for temperature reading time
        sleep_ms(20);
        err = i2c_read_blocking(self->inst, self->i2c_addr, &(data_out[0]), 2, false);

        // Equations are based off of SI7021 datasheet, to convert from 16 bit value to degrees C
        data_buffer = data_out[1] | (data_out[0] << 8);
        data_float = ((data_buffer*175.72)/65536) - 46.85;

        // If err is PICO_ERROR_GENERIC there is a broken connection between the SI7021 sensor
        // and the i2c line. What should be returned in the event of this failure?
        if(err == PICO_ERROR_GENERIC){
            printf("\nError Reading: %d", err);
        }
    } else {
        printf("\nError Writing: %d", err);
    }
    return data_float;
}

static void si7021_read_revision(struct Pico_Si7021 *self) {
    // static uint16_t cmd_data = SI7021_FIRMVERS_CMD;
    uint8_t cmd_data_array[2];
    cmd_data_array[0] = (uint8_t)(SI7021_FIRMVERS_CMD & 0x00FF);
    cmd_data_array[1] = (uint8_t)((SI7021_FIRMVERS_CMD & 0xFF) >> 8);
    
    uint16_t data_buffer=0;

    int err;

    absolute_time_t until_time = delayed_by_ms(get_absolute_time(), 500);
    printf("\nWriting to Temp Sensor");
    err = i2c_write_blocking(self->inst, self->i2c_addr, &cmd_data_array[0], 2, true);

    if(err!=PICO_ERROR_GENERIC) {
        // absolute_time_t until_time = delayed_by_ms(get_absolute_time(), 100);
        printf("\nReading from Temp Sensor");
        // i2c_read_blocking_until(self->inst, self->i2c_addr, &(self->data_out), 2, false, until_time);
        err = i2c_read_blocking(self->inst, self->i2c_addr, &(self->data_out[0]), 1, false);
        self->revision = self->data_out[0];
    } else {
        printf("\nReturn: %d", err);
    }
    // self->data_out = &cmd_data;
    // i2c_write_blocking(self->inst, self->i2c_addr, self->data_out, 2, true);
    // i2c_read_blocking(self->inst, self->i2c_addr, &(self->data_in), 1, false);
}