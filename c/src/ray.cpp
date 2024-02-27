#include "ray.h"
#include "vector.h"

Point rayAt(Ray* ray, f32 t)
{
  Vec3f32 scaledDir;
  scaleVec3f32(&scaledDir, &ray->dir, t);
  Point res;
  addVec3f32(&res, &ray->orig, &scaledDir);

  return res;
}
