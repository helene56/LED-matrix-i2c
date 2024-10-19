#ifndef LED
#define LED

#include "hardware/i2c.h"
#include <cstdint>
#include <array>

extern i2c_inst_t* I2C_PORT;
extern const std::uint8_t ATtiny_address;

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

void led_row(int row, std::uint8_t *color, size_t lenght);
// can only set r, g or b to a value
std::array<std::uint8_t, 2> set_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);
// set coordinate and r g b each to a value
void set_color_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);

void clear_pixel(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b);


#endif