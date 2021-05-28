/**Library to communicate with the si7021 temperature and humidity sensor,
 * designed after the adafruit arduino library.
 * 
 */

// Includes
#include "pico/stdlib.h"
#include "si7021_picopi.h"
#include "hardware/i2c.h"
#include <pico/binary_info.h>

uint8_t si7021_init(Pico_Si7021 *self, i2c_inst_t *i2c) {
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    self->inst = i2c;
    self->i2c_addr = SI7021_DEFAULT_ADDRESS;
    self->sernum_a = 0;
    self->sernum_b = 0;

    printf("\nI2C Bus Initialized For Si7021\nAddress:%d", self->i2c_addr);
}

float si7021_read_humidity(Pico_Si7021 *self) {
    static uint8_t cmd_data = SI7021_MEASRH_HOLD_CMD;
    self->data_out = &cmd_data;

    absolute_time_t until_time = delayed_by_ms(get_absolute_time(), 100);
    printf("\nWriting to Temp Sensor");
    i2c_write_blocking_until(self->inst, self->i2c_addr, self->data_out, 1, false, until_time);

    until_time = delayed_by_ms(get_absolute_time(), 100);
    printf("\nReading from Temp Sensor");
    i2c_read_blocking_until(self->inst, self->i2c_addr, &(self->data_in), 1, false, until_time);
}

static void si7021_read_revision(Pico_Si7021 *self) {
    static uint16_t cmd_data = SI7021_FIRMVERS_CMD;
    self->data_out = &cmd_data;
    i2c_write_blocking(self->inst, self->i2c_addr, self->data_out, 2, true);
    i2c_read_blocking(self->inst, self->i2c_addr, &(self->data_in), 1, false);
}