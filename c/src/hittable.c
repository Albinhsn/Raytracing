#include "hittable.h"
#include "vector.h"
#include <math.h>

bool scatterMetal(Metal metal, Ray* rayIn, HitRecord* rec, Color* attenuation, Ray* scattered)
{
  Vec3f32 reflected = reflectVec3f32(normalizeVec3f32(rayIn->dir), rec->normal);

  *scattered        = (Ray){.dir = addVec3f32(reflected, scaleVec3f32(randomUnitVector(), metal.fuzz)), .orig = rec->p};
  *attenuation      = metal.albedo;
  return dotVec3f32(scattered->dir, rec->normal) > 0;
}

bool scatterLambertian(Lambertian lambertian, HitRecord* rec, Color* attenuation, Ray* scattered)
{
  Vec3f32 direction = addVec3f32(rec->normal, randomUnitVector());
  if (nearZeroVec3f32(direction))
  {
    direction = rec->normal;
  }
  *scattered   = (Ray){.dir = direction, .orig = rec->p};
  *attenuation = lambertian.albedo;
  return true;
}

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
  rec->mat           = sphere->mat;

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
