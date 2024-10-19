#include "led.h"
#include "snake.h"

void Snake::move_head()
{
    clear_pixel(start_x, start_y, 0, 1, 0);
    if (start_y < 7)
    {
        start_y += 1;
    }
    else
    {
        start_y = 0;
    }
    set_color_pixel(start_x, start_y, 0, 255, 0);
    
    // if (start_y < 7)
    // {
    //     start_y += 1;
    // }
    // else
    // {
    //     start_y = 0;
    // }
}

