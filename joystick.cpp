#include "joystick.h"
#include <stdio.h>

void read_joystick(std::uint8_t *buffer_dst)
{

    std::uint8_t register_address = 0x2B; // Address of PORTD

    // Step 1: Send the register address (PORTD) to the ATTINY88
    int result = i2c_write_blocking(I2C_PORT, ATtiny_address, &register_address, 1, true); // true for 'repeated start'
    if (result < 0)
    {
        printf("I2C write failed\n");
        return;
    }
    // printf("I2C write result: %d\n", result);

    sleep_us(10);

    result = i2c_read_blocking(I2C_PORT, ATtiny_address, buffer_dst, 
                                   1, false);

    if (result < 0)
    {
        printf("I2C read failed\n");
    }
    // printf("I2C read result: %d\n", result);

    // else
    // {
    //     printf("Data read succesfully\n");
    // }
}