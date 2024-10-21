# Define the static library target
add_library(mandelbrot STATIC)

# Find and link FFmpeg libraries
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libswscale
    libavutil
)

# Include FFmpeg headers and link libraries
target_include_directories(mandelbrot PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(mandelbrot PRIVATE PkgConfig::FFMPEG)

# Add pkg-config flags
target_compile_options(mandelbrot PRIVATE ${FFMPEG_CFLAGS_OTHER})

# Add source files to the mandelbrot target
target_sources(mandelbrot PRIVATE mandelbrot.hpp mandelbrot.cpp png.hpp png.cpp number.hpp)

# Set the include directories for mandelbrot
target_include_directories(mandelbrot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
