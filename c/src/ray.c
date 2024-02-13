#include "ray.h"
#include "vector.h"

Point rayAt(Ray* ray, f32 t)
{
  return addVec3f32(ray->dir, scaleVec3f32(ray->dir, t));
}
