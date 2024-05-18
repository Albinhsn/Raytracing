#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linear_component) {
  return sqrt(linear_component);
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
  auto scale = 1.0 / samples_per_pixel;

  auto r = linear_to_gamma(pixel_color.x() * scale);
  auto g = linear_to_gamma(pixel_color.y() * scale);
  auto b = linear_to_gamma(pixel_color.z() * scale);

  if (r != r)
    r = 0.0;
  if (g != g)
    g = 0.0;
  if (b != b)
    b = 0.0;

  static const interval intensity(0.000, 0.999);

  out << static_cast<int>(256 * intensity.clamp(r)) << " "
      << static_cast<int>(256 * intensity.clamp(g)) << " "
      << static_cast<int>(256 * intensity.clamp(b)) << "\n";
}

#endif
