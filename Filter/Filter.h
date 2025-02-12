#ifndef CPP_HSE_FILTER_H
#define CPP_HSE_FILTER_H

#include <algorithm>
#include <ctype.h>
#include <memory>
#include <stdexcept>

#include "../image/image.h"
#include "../Parser/Parser.h"
#include "../Reader_and_Writer/Utilits.h"

namespace filter {
class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;
    virtual Image Apply(const Image& image) const = 0;

protected:
    template <typename T>
    std::vector<T> GetPixel(const std::vector<std::vector<T>>& matrix, const Image& img, const size_t& x,
                            const size_t& y) const;
};

class Sharpening : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class Negative : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class Grayscale : public Filter {
public:
    Image Apply(const Image& image) const override;
};

class Edge : public Filter {
public:
    void SetThreshold(double threshold);
    Image Apply(const Image& image) const override;

private:
    double threshold_;
};

class Crop : public Filter {
public:
    void SetHeight(size_t height);
    void SetWidth(size_t width);
    Image Apply(const Image& image) const override;

private:
    size_t height_;
    size_t width_;
};

class InvertedCircle : public Filter {
public:
    Image Apply(const Image& image) const override;
};

std::unique_ptr<filter::Filter> CreateFilter(const parser::Token& token);

uint8_t Clamp(int colour, const uint8_t max_colour = 255);

}  // namespace filter

#endif  // CPP_HSE_FILTER_H