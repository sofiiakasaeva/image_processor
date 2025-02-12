#include "image.h"

void Image::CheckHeightAndWidth(size_t height, size_t width) const {
    if (height <= 0 || width <= 0) {
        throw std::invalid_argument("Image dimensions must be positive.");
    }
}

Image::Image(size_t width, size_t height) : height_(height), width_(width) {
    CheckHeightAndWidth(height, width);
    pixel_colours_.resize(height);
    for (auto& row : pixel_colours_) {
        row.resize(width);
    }
}

Image::Image(const std::vector<std::vector<Colour>>& data) {
    height_ = data.size();
    if (height_ > 0) {
        width_ = data[0].size();
    } else {
        width_ = 0;
    }
    CheckHeightAndWidth(height_, width_);
    pixel_colours_ = data;
}

Image::Image(const Image& image) : height_(image.height_), width_(image.width_), pixel_colours_(image.pixel_colours_) {
}

size_t Image::GetHeight() const {
    return height_;
}

size_t Image::GetWidth() const {
    return width_;
}

const std::vector<std::vector<Colour>>& Image::GetPixelData() const {
    return pixel_colours_;
}

Colour& Image::GetColour(size_t x, size_t y) {
    if (x >= height_ || y >= width_) {
        throw std::out_of_range("Pixel coordinates out of range.");
    }
    return pixel_colours_[x][y];
}

const Colour& Image::GetColour(size_t x, size_t y) const {
    if (x >= height_ || y >= width_) {
        throw std::out_of_range("Pixel coordinates out of range.");
    }
    return pixel_colours_[x][y];
}

void Image::SetColor(size_t x, size_t y, const Colour& color) {
    if (x >= height_ || y >= width_) {
        throw std::out_of_range("Pixel coordinates out of range.");
    }
    pixel_colours_[x][y] = color;
}