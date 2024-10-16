#include "joystick.h"
#include <stdio.h>

void read_joystick(std::uint8_t *buffer_dst)
{
    int result = i2c_read_blocking(I2C_PORT, ATtiny_address, buffer_dst, 
                                   sizeof(buffer_dst), false);

    if (result < 0)
    {
        printf("I2C read failed\n");
    }
    else
    {
        printf("Data read succesfully\n");
    }
}