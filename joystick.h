#ifndef JOYSTICK
#define JOYSTICK

#include "hardware/i2c.h"
#include <cstdint>

extern i2c_inst_t* I2C_PORT;
extern const std::uint8_t ATtiny_address;
extern const std::uint8_t JOYSTICK_REG;
std::uint8_t read_joystick();
void check_joystick(std::uint8_t joystick_state);

#endif