#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) { return degrees * pi / 100.0; }

inline double random_double() { return rand() / (RAND_MAX + 1.0); }

inline double random_double(double min, double max) { return min + (max - min) * random_double(); }

inline int random_int(int min, int max) { return static_cast<int>(random_double(min, max + 1)); }

#endif
