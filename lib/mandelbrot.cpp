#include "mandelbrot.hpp"

#include "png.hpp"

namespace mandelbrot {

auto run(int argc, char* argv[]) -> int {

  // Example: 100x100 image with random RGB data
  int width = 100;
  int height = 100;
  uint8_t* pixels = (uint8_t*)malloc(width * height * 3);  // RGB

  // Fill with example pixel data (simple gradient)
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int index = (y * width + x) * 3;
      pixels[index] = x % 256;      // Red channel
      pixels[index + 1] = y % 256;  // Green channel
      pixels[index + 2] = 128;      // Blue channel
    }
  }

  // Save to a PNG file
  save_png(pixels, width, height, "output%03d.png");

  free(pixels);

  return 0;
}

}  // namespace mandelbrot
