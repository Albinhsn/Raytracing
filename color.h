#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto scale = 1.0 / samples_per_pixel;

    auto r = pixel_color.x() * scale;
    auto g = pixel_color.y() * scale;
    auto b = pixel_color.z() * scale;

    static const interval intensity(0.000, 0.999);

    out << static_cast<int>(256 * intensity.clamp(r)) << " " << static_cast<int>(256 * intensity.clamp(g)) << " "
        << static_cast<int>(256 * intensity.clamp(b)) << "\n";
}

#endif
