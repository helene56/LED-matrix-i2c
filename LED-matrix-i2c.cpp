#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"

#include <cstdint>
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define POWER_PIN 22
// address to ATtiny which interfaces with the led matrix
#define ATtiny_address 0x46

// send data to the led matrix
void send_led_data(std::uint8_t *data, size_t lenght);
// clear row
void clear_row(int row);
// clear whole matrix
void clear_matrix();
// blink
void blink_led(std::uint8_t *color, std::uint8_t *clear, int sleep_seconds);
// initialize the i2c communication
void initialize_i2c();

void led_row(int row, std::uint8_t *color, size_t lenght);

void set_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);

int main()
{
    stdio_init_all();
    // should use a transistor with this pin as well maybe..
    // gpio_init(POWER_PIN);
    // gpio_set_dir(POWER_PIN, GPIO_OUT);
    // gpio_put(POWER_PIN, 1);
    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    initialize_i2c();
    
    
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // example colors
    std::uint8_t green[2] {9, 63};
    std::uint8_t clear[2] {0, 0x00};
    std::uint8_t clear2[2] {4, 0x00};
    // 0 brightness, 63 full brightness
    std::uint8_t red[2] {0, 63};
    // the 5th led in the first row, turned fully on
    std::uint8_t red2[2] {4, 63};
    clear_matrix();
    set_pixel(2, 4, 1, 0, 0);
    while (true) {
        // clear_matrix();
        // Example to turn on the Pico W LED
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        printf("Hello, world!\n");

        blink_led(red, clear, 250);
        blink_led(red2, clear2, 250);
    }
}

void send_led_data(std::uint8_t *data, size_t lenght)
{
    int result = i2c_write_blocking(I2C_PORT, ATtiny_address, data, lenght, false);

    if (result < 0)
    {
        printf("I2C write failed\n");
    }
    else
    {
        printf("Data sent succesfully\n");
    }
    
}

void led_row(int row, std::uint8_t *color, size_t lenght)
{
    row = row * 24;
    for (std::uint8_t i = row; i <= 23+row; i++)
    {
        send_led_data(color, lenght);
    }
}

void clear_row(int row)
{
    row = row * 24;
    for (std::uint8_t i = row; i <= 23+row; i++)
    {
        std::uint8_t clear[2] {i, 0x00};
        send_led_data(clear, 2);
    }
}

void clear_matrix()
{
    for (int i = 0; i <= 8; i++)
    {
        clear_row(i);
    }
}

void blink_led(std::uint8_t *color, std::uint8_t *clear, int sleep_seconds)
{
    send_led_data(color, 2);
    sleep_ms(sleep_seconds);
    send_led_data(clear, 2);
    sleep_ms(sleep_seconds);
}

void set_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b)
{   
    int shift {0};
    if (r)
    {
        shift = 0;
    }
    else if (g)
    {
        shift = 8;
    }
    else if (b)
    {
        shift = 16;
    }

    int row = x * 24;
    int col = shift + y;

    std::uint8_t placement = row + col;
    std::uint8_t color[2] {placement, 63};
    printf("placement %d\n", placement);
    // for now always just use full intensity
    send_led_data(color, sizeof(color));

}

void initialize_i2c()
{
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    clear_matrix();
}