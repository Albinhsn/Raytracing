#ifndef RAY_H
#define RAY_H
#include "vector.h"

struct Ray
{
  Point   orig;
  Vec3f32 dir;
};

typedef struct Ray Ray;

Point              rayAt(Ray* ray, f32 t);

#endif
