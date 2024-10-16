#ifndef READ_JOYSTICK
#define READ_JOYSTICK

#include "hardware/i2c.h"
#include <cstdint>

extern i2c_inst_t* I2C_PORT i2c0;
extern const std::uint8_t ATtiny_address;
void read_joystick(std::uint8_t *buffer_dst);

#endif