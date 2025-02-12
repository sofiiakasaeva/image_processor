#include "image_processor.h"

int main(int argc, char** argv) {
    try {
        std::vector<parser::Token> tokens = Parse(argc, argv);
        Image img = GetImage(tokens[0].name);
        img = ApplyFilter(img, tokens);
        WriteImage(img, tokens[1].name);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

std::vector<parser::Token> Parse(int argc, char** argv) {
    std::vector<parser::Token> tokens = parser::Parse(argc, argv);
    if (tokens.size() < 2) {
        throw std::invalid_argument("Not enough arguments");
    }
    return tokens;
}

Image GetImage(const std::string& path) {
    reading_and_writing::Reader reader(path);
    Image image = reader.Read();
    return image;
}

void WriteImage(const Image& image, const std::string& path) {
    reading_and_writing::Writer writer(path);
    writer.Write(image);
}

Image ApplyFilter(Image& image, const std::vector<parser::Token>& tokens) {
    for (size_t i = 2; i < tokens.size(); ++i) {
        image = filter::CreateFilter(tokens[i])->Apply(image);
    }
    return image;
}