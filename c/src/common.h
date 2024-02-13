#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdlib.h>

#include <time.h>
typedef uint8_t       u8;
typedef uint16_t      u16;
typedef uint32_t      u32;
typedef unsigned long u64;

typedef float         f32;
typedef double        f64;

typedef int8_t        i8;
typedef int16_t       i16;
typedef int           i32;
typedef int64_t       i64;

typedef struct Vec3f32;

#define MIN(x, y)                        (x < y ? x : y)
#define MAX(x, y)                        (x < y ? y : x)
#define PI                               3.14159265358979323846 /* pi */

#define DEGREES_TO_RADIANS(d)            (d * PI / 180.0)
#define RANDOM_DOUBLE                    rand() / (RAND_MAX + 1.0)
#define RANDOM_DOUBLE_IN_RANGE(min, max) (min + (max - min) * RANDOM_DOUBLE())

#define RED                              ((struct Vec3f32){1.0f, 0, 0})
#define YELLOW                           ((struct Vec3f32){1.0f, 1.0f, 0})
#define GREEN                            ((struct Vec3f32){0, 1.0f, 0})
#define CYAN                             ((struct Vec3f32){0, 1.0f, 1.0f})
#define PURPLE                           ((struct Vec3f32){1.0f, 0, 1.0f})
#define BLUE                             ((struct Vec3f32){1.0f, 0, 0})
#define WHITE                            ((struct Vec3f32){1.0f, 1.0f, 1.0f})
#define BLACK                            ((struct Vec3f32){0, 0, 0})
#define SOMEBLUE                         ((struct Vec3f32){0.5f, 0.7f, 1.0f})

#endif
