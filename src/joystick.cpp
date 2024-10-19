#include "joystick.h"
#include <stdio.h>

std::uint8_t read_joystick()
{

    std::uint8_t joystick_reg = JOYSTICK_REG; // Address of PORTD
    std::uint8_t joystick_state {0};
    // write to the joystick register
    int result = i2c_write_blocking(I2C_PORT, ATtiny_address, &joystick_reg, 1, true); // true for 'repeated start'
    if (result < 0)
    {
        printf("I2C write failed\n");
    }
    // read the byte at the previous write location, then store the result in joystick_state
    result = i2c_read_blocking(I2C_PORT, ATtiny_address, &joystick_state, 
                                   1, false);

    if (result < 0)
    {
        printf("I2C read failed\n");
    }

    return joystick_state;
}

void check_joystick(std::uint8_t joystick_state)
{
    // note the directions will defer if the HAT is turned, 
    // so print statement is set to subjective values
    if (joystick_state & 0x01)
    {
        // 0000'0001
        printf("joystick down\n");
    }
    if (joystick_state & 0x02)
    {
        // 0000'0010
        printf("joystick right\n");
    }
    if (joystick_state & 0x04)
    {
        //0000'0100
        printf("joystick up\n");
    }
    if (joystick_state & 0x08)
    {
        //0000'1000
        printf("joystick pressed\n");
    }
    if (joystick_state & 0x10)
    {
        // 0001'0000
        printf("joystick left\n");
    }


}