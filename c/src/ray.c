#include "ray.h"
#include "vector.h"
#include <stdio.h>

Point rayAt(Ray* ray, f32 t)
{
  return addVec3f32(ray->orig, scaleVec3f32(ray->dir, t));
}
