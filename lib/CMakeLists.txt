# Define the static library target
add_library(mandelbrot STATIC)

# Add source files to the mandelbrot target
target_sources(mandelbrot PRIVATE mandelbrot.hpp mandelbrot.cpp)

# Set the include directories for mandelbrot
target_include_directories(mandelbrot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
