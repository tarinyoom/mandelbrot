#include <cstdint>

namespace mandelbrot {

void save_png(const uint8_t* pixels, int width, int height,
              const char* filename);

}
