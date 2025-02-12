#ifndef CPP_HSE_COLOUR_H
#define CPP_HSE_COLOUR_H

#include <cstdint>

struct Colour {
    Colour() = default;
    Colour(uint8_t red, uint8_t green, uint8_t blue);
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    void SetColours(uint8_t red, uint8_t green, uint8_t blue);
};

#endif  // CPP_HSE_COLOUR_H