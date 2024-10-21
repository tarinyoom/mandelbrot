#include "mandelbrot.hpp"

#include <iostream>

#include "number.hpp"
#include "png.hpp"

const int MAX_ITERS = 100;

namespace mandelbrot {

auto included(ComplexNumber c) -> bool {
  auto p = ComplexNumber(0.0, 0.0);
  for (auto i = 0; i < MAX_ITERS; i++) {
    if (norm2(p) >= 4) {
      return false;
    }

    p = p * p + c;
  }
  return true;
}

auto run(int argc, char* argv[]) -> int {
  int width = 1000;
  int height = 600;
  constexpr double pixels_per_unit = 250.0;
  constexpr double pixel_size = 1 / pixels_per_unit;
  std::vector<uint8_t> pixels(width * height * 3, 0);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto p =
          ComplexNumber((x - static_cast<double>(width) / 2.0) * pixel_size,
                        (y - static_cast<double>(height) / 2.0) * pixel_size);
      uint8_t color = included(p) ? 255 : 0;
      int index = (y * width + x) * 3;
      pixels[index] = color;      // Red channel
      pixels[index + 1] = color;  // Green channel
      pixels[index + 2] = color;  // Blue channel
    }
  }

  // Save to a PNG file
  save_png(pixels, width, height, "output%03d.png");

  return 0;
}

}  // namespace mandelbrot
