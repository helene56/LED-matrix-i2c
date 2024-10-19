#ifndef SNAKE
#define SNAKE

#include "led.h"

class Snake 
{
private:
    int start_x {3};
    int start_y {3};
public:
    void head() {set_color_pixel(start_x, start_y, 0, 255, 0);};
    void move_head();
};

#endif