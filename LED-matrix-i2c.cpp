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
void blink_led(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b, 
               int sleep_seconds);
// initialize the i2c communication
void initialize_i2c();

void led_row(int row, std::uint8_t *color, size_t lenght);
// can only set r, g or b to a value
void set_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);

void clear_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);

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
    set_pixel(2, 4, 255, 0, 0);
    sleep_ms(10000);
    set_pixel(6, 0, 0, 0, 255);
    while (true) {
        // clear_matrix();
        // Example to turn on the Pico W LED
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        printf("Hello, world!\n");
        blink_led(0, 0, 100, 0, 0, 250);
        blink_led(0, 4, 255, 0, 0, 250);
        
        // blink_led(red, clear, 250);
        // blink_led(red2, clear2, 250);
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

void blink_led(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b, int sleep_seconds)
{
    set_pixel(x, y, r, g, b);
    sleep_ms(sleep_seconds);
    clear_pixel(x, y, r, g, b);
    sleep_ms(sleep_seconds);
}

void set_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b)
{   
    // use bitwise operators instead
    // turn rgb into a single flag
    std::uint8_t rgb_flag {static_cast<std::uint8_t>(((r ? 1: 0) << 2) | ((g ? 1: 0) << 1) | ((b ? 1: 0) << 0))};
    int shift {0};

    shift = (rgb_flag == 0b100) ? 0 :
            (rgb_flag == 0b010) ? 8 :
            (rgb_flag == 0b001) ? 16 : -1; // -1 for error handling

    const int row = x * 24;
    const int col = shift + y;
    
    const std::uint8_t placement = row + col;
    printf("shift: %d\n", shift);
    printf("placement: %d\n", placement);
    // sets the intensity to the first value that is not 0
    const std::uint8_t intensity {static_cast<std::uint8_t>((r != 0) ? r : ((g != 0) ? g : ((b != 0) ? b : 0)))};
    // max value can only be 63 because the registers only take up to 63
    std::uint8_t color[2] {placement, static_cast<std::uint8_t>(((intensity * 63) / 255))};
    // for now always just use full intensity
    send_led_data(color, sizeof(color));

}

void clear_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b)

{
    std::uint8_t rgb_flag {static_cast<std::uint8_t>(((r ? 1: 0) << 2) | ((g ? 1: 0) << 1) | ((b ? 1: 0) << 0))};
    int shift {0};

    shift = (rgb_flag * 8) - 32;
    // 2. turn rgb values into intensity
    const int row = x * 24;
    const int col = shift + y;

    const std::uint8_t placement = row + col;

    std::uint8_t clear_color[2] {placement, 0x00};
    // for now always just use full intensity
    send_led_data(clear_color, sizeof(clear_color));
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