#include <cstdint>
#include <string>
#include <vector>

namespace mandelbrot {

void save_png(const std::vector<uint8_t> pixels, int width, int height,
              const std::string& filename);

}
