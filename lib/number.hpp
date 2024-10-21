#pragma once

namespace mandelbrot {

struct ComplexNumber {
  double x;
  double y;

  ComplexNumber(double real = 0.0, double imag = 0.0) : x(real), y(imag) {}
};

auto norm2(ComplexNumber z) -> double { return z.x * z.x + z.y * z.y; }

auto operator+(ComplexNumber u, ComplexNumber v) -> ComplexNumber {
  return ComplexNumber(u.x + v.x, u.y + v.y);
}

auto operator*(ComplexNumber u, ComplexNumber v) -> ComplexNumber {
  return ComplexNumber(u.x * v.x - u.y * v.y, u.x * v.y + u.y * v.x);
}

}  // namespace mandelbrot
