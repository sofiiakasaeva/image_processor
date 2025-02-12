#ifndef CPP_HSE_WRITER_H
#define CPP_HSE_WRITER_H

#include <algorithm>
#include <cerrno>
#include <fstream>
#include <string>
#include <utility>

#include "../image/image.h"
#include "Reader.h"
#include "Utilits.h"

namespace reading_and_writing {
class Writer {
public:
    explicit Writer(std::string path);
    void Write(const Image& image);

private:
    void WriteBMPHeader(unsigned char* bmp_header, size_t size_of_file);
    void WriteDIBHeader(unsigned char* dib_header, size_t width, size_t height);

    template <typename T>
    void NumberToBytes(T number, unsigned char* bytes);

    std::string filename_;
};
}  // namespace reading_and_writing
#endif  // CPP_HSE_WRITER_H