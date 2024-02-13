#include "hittable.h"
#include "vector.h"
#include <math.h>

static inline void calculateFaceNormal(HitRecord* record, Ray* ray, Vec3 outwardNormal)
{
  record->frontFace = dotVec3f32(ray->dir, outwardNormal) < 0;
  record->normal    = record->frontFace ? outwardNormal : CREATE_VEC3f32(-outwardNormal.x, -outwardNormal.y, -outwardNormal.z);
}

bool hitSphere(Sphere* sphere, Ray* ray, Interval rayt, HitRecord* rec)
{
  Vec3 oc           = subVec3f32(ray->orig, sphere->center);
  f32  a            = lengthSquaredVec3f32(ray->dir);

  f32  half_b       = dotVec3f32(oc, ray->dir);
  f32  c            = lengthSquaredVec3f32(oc) - sphere->radius * sphere->radius;

  f32  discriminant = half_b * half_b - a * c;

  if (discriminant < 0)
  {
    return false;
  }

  f32 sqrtd = sqrt(discriminant);
  f32 root  = (-half_b - sqrtd) / a;
  if (!intervalSurrounds(rayt, root))
  {
    f32 root = (-half_b + sqrtd) / a;
    if (!intervalSurrounds(rayt, root))
    {
      return false;
    }
  }

  rec->t             = root;
  rec->p             = rayAt(ray, root);
  Vec3 outwardNormal = scaleVec3f32(subVec3f32(rec->p, sphere->center), 1 / sphere->radius);
  calculateFaceNormal(rec, ray, outwardNormal);

  return true;
}

bool calculateRayIntersection(Hittable* hittableObjects, i32 length, Ray* ray, Interval rayt, HitRecord* rec)
{
  bool hit        = false;
  f32  closestHit = rayt.max;

  for (i32 i = 0; i < length; i++)
  {
    Hittable obj = hittableObjects[i];
    switch (obj.type)
    {
    case SPHERE:
    {
      if (hitSphere(&obj.sphere, ray, CREATE_INTERVAL(rayt.min, closestHit), rec))
      {
        hit        = true;
        closestHit = rec->t;
      }
      break;
    }
    }
  }
  return hit;
}
