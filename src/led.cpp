#include "led.h"
#include <stdio.h>
#include "led.h"
#include <array>

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