#include "color.h"

Colour::Colour(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {
}

void Colour::SetColours(uint8_t new_red, uint8_t new_green, uint8_t new_blue) {
    red = new_red;
    green = new_green;
    blue = new_blue;
}