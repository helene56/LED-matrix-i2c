#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"

#include <cstdint>
#include <array>
// own header files
#include "joystick.h"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
i2c_inst_t* I2C_PORT {i2c0};
constexpr int I2C_SDA {8};
constexpr int I2C_SCL {9};
constexpr int POWER_PIN {16};
std::uint8_t PORTDstate {0x0B};
// constexpr std::uint8_t PORTDinput {0x0A};
// constexpr std::uint8_t PORTC7 {0x08};
// address to ATtiny which interfaces with the led matrix
constexpr std::uint8_t ATtiny_address {0x46};


void read_joystick();

void send_led_data_continue(std::uint8_t *data, size_t lenght);
// send data to the led matrix
void send_data(std::uint8_t *data, size_t lenght);
// clear row
inline void clear_row(std::uint8_t* buffer);
// clear whole matrix
void clear_matrix();
// clear matrix with a small delay to avoid the led staying on
void initialize_clear_matrix();
// blink
void blink_led(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b, 
               int sleep_seconds);
// initialize the i2c communication
void initialize_i2c();

void led_row(int row, std::uint8_t *color, size_t lenght);
// can only set r, g or b to a value
std::array<std::uint8_t, 2> set_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);
// set coordinate and r g b each to a value
void set_color_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);

void clear_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);

int main()
{
    stdio_init_all();
    // should use a transistor with this pin as well maybe..
    gpio_init(POWER_PIN);
    gpio_set_dir(POWER_PIN, GPIO_OUT);
    gpio_put(POWER_PIN, 1);
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
    // set_pixel(6, 0, 0, 0, 255);
    // set_pixel(7, 0, 0, 0, 10);
    // set_pixel(3, 0, 0, 255, 0);
    // set_pixel(3, 0, 255, 0, 0);
    // set_color_pixel(1, 0, 5, 255, 251);
    // set_color_pixel(1, 1, 66, 14, 150);

    // std::uint8_t byte {0b00000000};

    // write to portd to set the 3:7 to inputs (skip, they are by defeault set to 0)
    // mask bit 3 to 7, to isolate the joytstick inputs
    std::uint8_t joystickmask {0b11111000};
    std::uint8_t reg = 0x2B; // PORTD address
    std::uint8_t value = 0b11111000; // Enable pull-ups on bits 3-7
    // Enable pull-ups on bits 3-7
    // i2c_write_blocking(I2C_PORT, ATtiny_address, &reg, 1, true); // Send register address
    // i2c_write_blocking(I2C_PORT, ATtiny_address, &value, 1, true); // Write pull-up enable
    while (true) {
        // std::uint8_t joystick_data {0};
        // // Send the register address you want to read from
        // int result = i2c_write_blocking(I2C_PORT, ATtiny_address, &reg, 1, true);  // true to keep the connection open
        // if (result < 0) {
        //     printf("I2C write failed for reg 0x%02X\n", reg);
        // }
        // // Read the value from the register
        // result = i2c_read_blocking(I2C_PORT, ATtiny_address, &joystick_data, 1, false);  // false to close the connection after read
        // printf("Raw joystick value: 0x%02X\n", joystick_data);

        // std::uint8_t joystick_data;
        // read_joystick(&joystick_data);  // Read from register 0x2B (PORTD)
        // // Print raw value
        // printf("Raw joystick value: 0x%02X\n", joystick_data);
        // // Send register address to ATtiny88 (PORTD, 0x2B)
        // int result = i2c_write_blocking(I2C_PORT, ATtiny_address, &reg, 1, true); // Send register address
        // if (result < 0) {
        //     printf("I2C write failed for reg 0x%02X\n", reg);
        // }
        // // Add a small delay to avoid overwhelming the system
        // sleep_ms(200);

        
        // std::uint8_t joystick_data;
        // read_joystick(&joystick_data);  // Read from register 0x2B (PORTD)
        // joystick_data &= 0xF8;  // Mask to focus on bits 3 to 7 (0b11111000)

        // // Check each direction:
        // if (!(joystick_data & (1 << 3))) {
        //     printf("Joystick UP is pressed\n");
        // }
        // if (!(joystick_data & (1 << 4))) {
        //     printf("Joystick DOWN is pressed\n");
        // }
        // if (!(joystick_data & (1 << 5))) {
        //     printf("Joystick LEFT is pressed\n");
        // }
        // if (!(joystick_data & (1 << 6))) {
        //     printf("Joystick RIGHT is pressed\n");
        // }
        // if (!(joystick_data & (1 << 7))) {
        //     printf("Joystick ENTER is pressed\n");
        // }
        // // Send register address to ATtiny88 (PIND, 0x29)
        // int result = i2c_write_blocking(I2C_PORT, ATtiny_address, &reg, 1, true); // Send register address
        // if (result < 0) {
        //     printf("I2C write failed for reg 0x%02X\n", reg);
        // }

        // Read 1 byte from ATtiny88 (PIND)
        // result = i2c_read_blocking(I2C_PORT, ATtiny_address, &buffer_dst, 1, false); // Read 1 byte
        // if (result < 0) 
        // {
        //     printf("I2C read failed for reg 0x%02X\n", reg);
        // } else 
        // {
        //     // Mask the result to focus on bits 3-7 (joystick directions)
        //     buffer_dst &= 0b11111000;
        //     printf("PIND register value: 0x%02X\n", buffer_dst);

        //     // Check joystick directions by reading specific bits:
        //     if (!(buffer_dst & (1 << 3))) {
        //         printf("Joystick LEFT is pressed\n");
        //     }
        //     if (!(buffer_dst & (1 << 4))) {
        //         printf("Joystick RIGHT is pressed\n");
        //     }
        //     if (!(buffer_dst & (1 << 5))) {
        //         printf("Joystick UP is pressed\n");
        //     }
        //     if (!(buffer_dst & (1 << 6))) {
        //         printf("Joystick DOWN is pressed\n");
        //     }
        //     if (!(buffer_dst & (1 << 7))) {
        //         printf("Joystick ENTER (middle) is pressed\n");
        //     }
        // }

        // // Add a small delay to avoid overwhelming the system
        // sleep_ms(200);
    
        
        
    

        // std::uint8_t buffer_dst = 0xF8;
        // read_joystick(&buffer_dst);
        // // Print raw value
        // printf("Raw joystick value: 0x%02X\n", buffer_dst);
        // // Shift and mask to get joystick state
        // std::uint8_t joystick_state = (buffer_dst & joystickmask) >> 3;
        // // Check each joystick direction
        // if (joystick_state & (1 << 0))
        // {
        //     printf("Joystick direction 1 is pressed\n");
        // }
        // if (joystick_state & (1 << 1))
        // {
        //     printf("Joystick direction 2 is pressed\n");
        // }
        // if (joystick_state & (1 << 2))
        // {
        //     printf("Joystick direction 3 is pressed\n");
        // }
        // if (joystick_state & (1 << 3))
        // {
        //     printf("Joystick direction 4 is pressed\n");
        // }
        // if (joystick_state & (1 << 4))
        // {
        //     printf("Joystick direction 5 is pressed\n");
        // }
        // sleep_ms(1000); // Add a small delay to avoid overwhelming the system.
        // for (int i = 0; i < 100; ++i)
        // {
        //     printf("%d, i = %d\n", static_cast<int>(buffer_dst[i]), i);
        //     sleep_ms(1000);
        // }
        // read_joystick(pixel.data());
        // printf("%d\n", static_cast<int>(pixel[0]));
        // sleep_ms(1000);
        // Example to turn on the Pico W LED
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        printf("Hello, world!\n");
        blink_led(0, 0, 100, 0, 0, 250);
        blink_led(0, 4, 255, 0, 0, 250);
        
    }
}

void send_data(std::uint8_t *data, size_t lenght)
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


void initialize_clear_matrix()
{
    // small delay to prevent the led light from staying on even if it got cleared
    sleep_ms(30);
    clear_matrix();
}

void send_led_data_continue(std::uint8_t *data, size_t lenght)
{
    int result = i2c_write_blocking(I2C_PORT, ATtiny_address, data, lenght, true);

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
        send_data(color, lenght);
    }
}

inline void clear_row(std::uint8_t* buffer)
{
     for (std::uint8_t i = 0; i < 8; i++) {  
        buffer[i] = 0x00;          // Red component
        buffer[i + 8] = 0x00;      // Green component
        buffer[i + 16] = 0x00;     // Blue component

    }
}

void clear_matrix()
{
    std::uint8_t buffer[193] {0};
    for (int i = 0; i < 8; i++)
    {
        int row_start = i * 24;
        clear_row(buffer + row_start);
    }

    send_data(buffer, sizeof(buffer));
}

void blink_led(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b, int sleep_seconds)
{
    set_pixel(x, y, r, g, b);
    sleep_ms(sleep_seconds);
    clear_pixel(x, y, r, g, b);
    sleep_ms(sleep_seconds);
}

void set_color_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    set_pixel(x, y, r, 0, 0);
    set_pixel(x, y, 0, g, 0);
    set_pixel(x, y , 0, 0 ,b);
}

std::array<std::uint8_t, 2> set_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b)
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
    // sets the intensity to the first value that is not 0
    const std::uint8_t intensity {static_cast<std::uint8_t>((r != 0) ? r : ((g != 0) ? g : ((b != 0) ? b : 0)))};
    // max value can only be 63 because the registers only take up to 63
    std::array<std::uint8_t, 2> color {placement, static_cast<std::uint8_t>(((intensity * 63) / 255))};
    // for now always just use full intensity
    send_data(color.data(), color.size() * sizeof(std::uint8_t));
    return color;

}

void clear_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    std::uint8_t rgb_flag {static_cast<std::uint8_t>(((r ? 1: 0) << 2) | ((g ? 1: 0) << 1) | ((b ? 1: 0) << 0))};
    int shift {0};

    shift = (rgb_flag == 0b100) ? 0 :
            (rgb_flag == 0b010) ? 8 :
            (rgb_flag == 0b001) ? 16 : -1; // -1 for error handling
    // 2. turn rgb values into intensity
    const int row = x * 24;
    const int col = shift + y;

    const std::uint8_t placement = row + col;

    std::uint8_t clear_color[2] {placement, 0x00};
    // for now always just use full intensity
    send_data(clear_color, sizeof(clear_color));
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