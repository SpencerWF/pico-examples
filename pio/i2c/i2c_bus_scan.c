#include <stdio.h>

#include "pico/stdlib.h"
#include "pio_i2c.h"

#define PIN_SDA 4
#define PIN_SCL 5

bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

int main() {
    stdio_init_all();
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, PIN_SDA, PIN_SCL);

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
    sleep_ms(500);
    gpio_put(LED_PIN, 1);

    while(true) {
        printf("\nPIO I2C Bus Scan\n");
        printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

        for (int addr = 0; addr < (1 << 7); ++addr) {
            if (addr % 16 == 0) {
                printf("%02x ", addr);
            }
            // Perform a 0-byte read from the probe address. The read function
            // returns a negative result NAK'd any time other than the last data
            // byte. Skip over reserved addresses.
            int result;
            if (reserved_addr(addr))
                result = -1;
            else
                result = pio_i2c_read_blocking(pio, sm, addr, NULL, 0);

            printf(result < 0 ? "." : "@");
            printf(addr % 16 == 15 ? "\n" : "  ");
        }
        printf("Done.\n");
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
        gpio_put(LED_PIN, 1);
        sleep_ms(1000);
    }
    return 0;
}
