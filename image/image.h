#ifndef CPP_HSE_IMAGE_H
#define CPP_HSE_IMAGE_H

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "color.h"

class Image {
public:
    Image() = default;
    Image(size_t width, size_t height);
    explicit Image(const std::vector<std::vector<Colour>>& data);
    Image(const Image& image);
    ~Image() = default;

    size_t GetHeight() const;
    size_t GetWidth() const;
    const std::vector<std::vector<Colour>>& GetPixelData() const;
    Colour& GetColour(size_t x, size_t y);
    const Colour& GetColour(size_t x, size_t y) const;

    void SetColor(size_t x, size_t y, const Colour& color);

private:
    size_t height_ = 0;
    size_t width_ = 0;
    std::vector<std::vector<Colour>> pixel_colours_;

    void CheckHeightAndWidth(size_t height, size_t width) const;
};

#endif  // CPP_HSE_IMAGE_H