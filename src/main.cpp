#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"
// cpp header files
#include <cstdint>
#include <array>
// own header files
#include "joystick.h"
#include "led.h"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
i2c_inst_t* I2C_PORT {i2c0};
constexpr int I2C_SDA {8};
constexpr int I2C_SCL {9};

// address to ATtiny which interfaces with the led matrix
constexpr std::uint8_t ATtiny_address {0x46};
constexpr std::uint8_t JOYSTICK_REG {0xF2};

// initialize the i2c communication
void initialize_i2c();

int main()
{
    stdio_init_all();
    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    initialize_i2c();
    
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    initialize_clear_matrix();

    // std::array<std::uint8_t, 2> pixel = set_pixel(0, 0, 255, 0, 0);
    set_pixel(0, 0, 255, 0, 0);
    set_pixel(1, 1, 0, 0, 255);

    while (true) {
        
        // Example to turn on the Pico W LED
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        // printf("Hello, world!\n");
        std::uint8_t joystick_state {read_joystick()};

        check_joystick(joystick_state);
        sleep_ms(100);
        
    }
}

void initialize_i2c()
{
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

}