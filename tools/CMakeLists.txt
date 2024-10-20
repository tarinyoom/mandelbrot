# Define the executable target
add_executable(make_mandelbrot)

# Set the source file for the make_mandelbrot executable
set(TOOLS_FILES
    driver.cpp
)

# Add the source files to the make_mandelbrot executable
target_sources(make_mandelbrot PRIVATE ${TOOLS_FILES})

# Link the executable against the mandelbrot library
target_link_libraries(make_mandelbrot PRIVATE mandelbrot)
