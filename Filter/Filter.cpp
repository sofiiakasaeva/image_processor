#include "Filter.h"

template <typename T>
std::vector<T> filter::Filter::GetPixel(const std::vector<std::vector<T>>& matrix, const Image& img, const size_t& x,
                                        const size_t& y) const {
    T red = 0;
    T green = 0;
    T blue = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix.front().size(); ++j) {
            red += img.GetColour(std::clamp(static_cast<int>(i + x) - 1, static_cast<int>(0),
                                            static_cast<int>(img.GetHeight()) - 1),
                                 std::clamp(static_cast<int>(j + y) - 1, static_cast<int>(0),
                                            static_cast<int>(img.GetWidth()) - 1))
                       .red *
                   matrix[i][j];
            green += img.GetColour(std::clamp(static_cast<int>(i + x) - 1, static_cast<int>(0),
                                              static_cast<int>(img.GetHeight()) - 1),
                                   std::clamp(static_cast<int>(j + y) - 1, static_cast<int>(0),
                                              static_cast<int>(img.GetWidth()) - 1))
                         .green *
                     matrix[i][j];
            blue += img.GetColour(std::clamp(static_cast<int>(i + x) - 1, static_cast<int>(0),
                                             static_cast<int>(img.GetHeight()) - 1),
                                  std::clamp(static_cast<int>(j + y) - 1, static_cast<int>(0),
                                             static_cast<int>(img.GetWidth()) - 1))
                        .blue *
                    matrix[i][j];
        }
    }
    return {blue, green, red};
}

uint8_t filter::Clamp(int colour, const uint8_t max_colour) {
    if (colour < 0) {
        return 0;
    } else if (colour > max_colour) {
        return max_colour;
    }
    return static_cast<uint8_t>(colour);
}

Image filter::Sharpening::Apply(const Image& image) const {
    std::vector<std::vector<Colour>> new_data;
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Colour> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            std::vector<int> colours = GetPixel(matrix, image, i, j);
            row[j].blue = Clamp(colours[0]);
            row[j].green = Clamp(colours[1]);
            row[j].red = Clamp(colours[2]);
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

Image filter::Grayscale::Apply(const Image& image) const {
    std::vector<std::vector<Colour>> new_data;
    uint8_t common_colour = 0;
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Colour> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            common_colour =
                static_cast<uint8_t>(image::utils::GRAY_SCALE_RED * static_cast<float>(image.GetColour(i, j).red)) +
                static_cast<uint8_t>(image::utils::GRAY_SCALE_BLUE * static_cast<float>(image.GetColour(i, j).green)) +
                static_cast<uint8_t>(image::utils::GRAY_SCALE_GREEN * static_cast<float>(image.GetColour(i, j).blue));

            row[j] = {common_colour, common_colour, common_colour};
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

Image filter::Negative::Apply(const Image& image) const {
    std::vector<std::vector<Colour>> new_data;
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Colour> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            row[j] = {static_cast<uint8_t>(image::utils::BITS_PER_COLOUR - image.GetColour(i, j).red),
                      static_cast<uint8_t>(image::utils::BITS_PER_COLOUR - image.GetColour(i, j).green),
                      static_cast<uint8_t>(image::utils::BITS_PER_COLOUR - image.GetColour(i, j).blue)};
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

void filter::Crop::SetHeight(size_t height) {
    height_ = height;
}
void filter::Crop::SetWidth(size_t width) {
    width_ = width;
}

size_t Check(size_t old_value, size_t new_value) {
    if (new_value > old_value) {
        return old_value;
    } else {
        return new_value;
    }
}

Image filter::Crop::Apply(const Image& image) const {
    std::vector<std::vector<Colour>> new_data;
    size_t height_new = Check(image.GetHeight(), height_);
    size_t width_new = Check(image.GetWidth(), width_);
    for (size_t i = image.GetHeight() - height_new; i < image.GetHeight(); ++i) {
        std::vector<Colour> row(width_new);
        for (size_t j = 0; j < width_new; ++j) {
            row[j] = {static_cast<uint8_t>(image.GetColour(i, j).red),
                      static_cast<uint8_t>(image.GetColour(i, j).green),
                      static_cast<uint8_t>(image.GetColour(i, j).blue)};
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

void filter::Edge::SetThreshold(double threshold) {
    threshold_ = threshold;
}

Image filter::Edge::Apply(const Image& image) const {
    std::unique_ptr<Grayscale> ptr = std::make_unique<Grayscale>();
    std::vector<std::vector<Colour>> new_data = ptr->Apply(image).GetPixelData();
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            std::vector<int> colours = GetPixel(matrix, ptr->Apply(image), i, j);
            new_data[i][j].blue = Clamp(colours[0]);
            new_data[i][j].green = Clamp(colours[1]);
            new_data[i][j].red = Clamp(colours[2]);
            if (new_data[i][j].blue >= threshold_ * (image::utils::BITS_PER_COLOUR + image::utils::CNT_OF_PLANES)) {
                new_data[i][j] = {image::utils::BITS_PER_COLOUR, image::utils::BITS_PER_COLOUR,
                                  image::utils::BITS_PER_COLOUR};
            } else {
                new_data[i][j] = {1, 1, 1};
            }
        }
    }
    return Image(new_data);
}

Image filter::InvertedCircle::Apply(const Image& image) const {
    std::vector<std::vector<Colour>> new_data;
    size_t middle_height = image.GetHeight() / 2;
    size_t middle_width = image.GetWidth() / 2;
    size_t radius = std::min(image.GetWidth(), image.GetHeight()) / 4;
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Colour> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            if (((i - middle_height) * (i - middle_height) + (j - middle_width) * (j - middle_width)) >
                radius * radius) {
                if (std::rand() % 2 == 0) {
                    row[j] = {0, 0, 0};
                } else {
                    row[j] = {image::utils::BITS_PER_COLOUR, image::utils::BITS_PER_COLOUR,
                              image::utils::BITS_PER_COLOUR};
                }
            } else {
                if (i <= middle_height) {
                    row[j] = {static_cast<uint8_t>(image.GetColour(i, j).red),
                              static_cast<uint8_t>(image.GetColour(i, j).green),
                              static_cast<uint8_t>(image.GetColour(i, j).blue)};
                } else {
                    Colour inverted = new_data[image.GetHeight() - i][j];
                    new_data[image.GetHeight() - i][j] = {static_cast<uint8_t>(image.GetColour(i, j).red),
                                                          static_cast<uint8_t>(image.GetColour(i, j).green),
                                                          static_cast<uint8_t>(image.GetColour(i, j).blue)};
                    row[j] = inverted;
                }
            }
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

bool IsDigit(std::string width_height) {
    for (size_t i = 0; i < width_height.size(); ++i) {
        if (isdigit(width_height[i]) == 0) {
            return false;
        }
    }
    return true;
}

bool EdgeCheck(std::string arg) {
    if (arg[0] != '0' && arg[0] != '1') {
        return false;
    }
    if (arg.size() > 1 && arg[1] != '.') {
        return false;
    }
    for (size_t i = 2; i < arg.size(); ++i) {
        if (isdigit(arg[i]) == 0) {
            return false;
        }
    }
    return true;
}

std::unique_ptr<filter::Filter> filter::CreateFilter(const parser::Token& token) {
    const std::string& filter = token.name;
    if (filter == "-sharp") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Invalid argument for sharpening filter");
        }
        std::unique_ptr<Sharpening> ptr = std::make_unique<Sharpening>();
        return ptr;
    } else if (filter == "-neg") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Invalid argument for negative filter");
        }
        std::unique_ptr<Negative> ptr = std::make_unique<Negative>();
        return ptr;
    } else if (filter == "-gs") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Invalid argument for gray scale filter");
        }
        std::unique_ptr<Grayscale> ptr = std::make_unique<Grayscale>();
        return ptr;
    } else if (filter == "-crop") {
        if (token.args.size() != 2 || (!IsDigit(token.args[0])) || (!IsDigit(token.args[1]))) {
            throw std::invalid_argument("Invalid argument for crop filter");
        }
        std::unique_ptr<Crop> ptr = std::make_unique<Crop>();
        ptr->SetHeight(std::stoi(token.args[1]));
        ptr->SetWidth(std::stoi(token.args[0]));
        return ptr;
    } else if (filter == "-edge") {
        if (token.args.size() != 1 || (!EdgeCheck(token.args[1]))) {
            throw std::invalid_argument("Invalid argument for edge filter");
        }
        std::unique_ptr<Edge> ptr = std::make_unique<Edge>();
        ptr->SetThreshold(std::stod(token.args[0]));
        return ptr;
    } else if (filter == "-circle") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Invalid argument for gray scale filter");
        }
        std::unique_ptr<InvertedCircle> ptr = std::make_unique<InvertedCircle>();
        return ptr;
    } else {
        throw std::invalid_argument("Invalid filter" + filter);
    }
}