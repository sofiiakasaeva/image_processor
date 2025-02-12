#ifndef CPP_HSE_UTILITS_H
#define CPP_HSE_UTILITS_H

#include <iostream>
#include <string>
#include <vector>

namespace image::utils {
const int BMP_HEADER_SIZE = 14;
const int DIB_HEADER_SIZE = 40;
const int PADDING_SIZE = 4;
const int BYTES_PER_PIXEL = 3;
const int HEADER_WIDTH_OFFSET = 4;
const int HEADER_HEIGHT_OFFSET = 8;
const int FORMAT_SIZE_FIRST_POSITION = 0;
const int FORMAT_FILE_SECOND_POSITION = 1;
const int SIZE_POSITION = 2;
const int PIXS_POSITION = 10;
const int DIB_HEADER_SIZE_POSITION = 0;
const int CNT_OF_PLANES = 1;
const int CNT_OF_PLANES_POSITION = 12;
const int BITS_PER_PIXEL = 24;
const int BITS_PER_PIXEL_POSITION = 14;
const int BITS_PER_COLOUR = 255;
const float GRAY_SCALE_RED = 0.299;
const float GRAY_SCALE_BLUE = 0.587;
const float GRAY_SCALE_GREEN = 0.114;
const std::vector<char> FORMAT_BYTES = {'B', 'M'};
const std::vector<int> OFFSET_OF_BYTES = {8, 16, 24};
}  // namespace image::utils

#endif  // CPP_HSE_UTILITS_H