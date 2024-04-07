#include "hittable.h"
#include "vector.h"
#include <math.h>

bool scatterDielectric(Dielectric dielectric, Ray* rayIn, HitRecord* rec, Color* attenuation, Ray* scattered)
{

  *attenuation            = WHITE;
  f32     refractionRatio = rec->frontFace ? 1.0f / dielectric.ir : dielectric.ir;

  Vec3f32 unitDirection;
  normalizeVec3f32(&unitDirection, &rayIn->dir);

  Vec3f32 negDir        = NEG_VEC3f32(unitDirection);
  f32     cosTheta      = fmin(dotVec3f32(&negDir, &rec->normal), 1.0f);
  f32     sinTheta      = sqrt(1.0f - cosTheta * cosTheta);

  bool    cannotRefract = refractionRatio * sinTheta > 1.0f;
  Vec3f32 direction;

  if (cannotRefract || reflectanceVec3f32(cosTheta, refractionRatio) > RANDOM_DOUBLE)
  {
    reflectVec3f32(&direction, &unitDirection, &rec->normal);
  }
  else
  {
    refractVec3f32(&direction, &unitDirection, &rec->normal, refractionRatio);
  }

  *scattered = (Ray){.orig = rec->p, .dir = direction};

  return true;
}

bool scatterMetal(Metal metal, Ray* rayIn, HitRecord* rec, Color* attenuation, Ray* scattered)
{
  Vec3f32 normDir;
  normalizeVec3f32(&normDir, &rayIn->dir);

  Vec3f32 reflected;
  reflectVec3f32(&reflected, &normDir, &rec->normal);

  Vec3f32 fuzzedUnit;
  Vec3f32 unit = randomUnitVector();
  scaleVec3f32(&fuzzedUnit, &unit, metal.fuzz);

  Vec3f32 dir;
  addVec3f32(&dir, &reflected, &fuzzedUnit);
  *scattered = (Ray){
      .orig = rec->p, //
      .dir  = dir     //
  };
  *attenuation = metal.albedo;
  return dotVec3f32(&scattered->dir, &rec->normal) > 0;
}

bool scatterLambertian(Lambertian lambertian, HitRecord* rec, Color* attenuation, Ray* scattered)
{
  Vec3f32 direction;
  Vec3f32 randomUnit = randomUnitVector();
  addVec3f32(&direction, &rec->normal, &randomUnit);

  scattered->orig = rec->p;
  scattered->dir  = nearZeroVec3f32(direction) ? rec->normal : direction;
  *attenuation    = lambertian.albedo;

  return true;
}

static inline void calculateFaceNormal(HitRecord* record, Ray* ray, Vec3f32 outwardNormal)
{
  record->frontFace = dotVec3f32(&ray->dir, &outwardNormal) < 0;
  record->normal    = record->frontFace ? outwardNormal : CREATE_VEC3f32(-outwardNormal.x, -outwardNormal.y, -outwardNormal.z);
}

bool hitSphere(Sphere* sphere, Ray* ray, Interval rayt, HitRecord* rec)
{
  Vec3f32 oc;
  subVec3f32(&oc, &ray->orig, &sphere->center);

  f32 half_b       = dotVec3f32(&oc, &ray->dir);
  f32 c            = lengthSquaredVec3f32(&oc) - sphere->radius * sphere->radius;

  f32 a            = lengthSquaredVec3f32(&ray->dir);

  f32 discriminant = half_b * half_b - a * c;

  if (discriminant < 0)
  {
    return false;
  }

  f32 sqrtd = sqrt(discriminant);
  f32 root  = (-half_b - sqrtd) / a;
  if (!INTERVAL_SURROUNDS(rayt, root))
  {
    root = (-half_b + sqrtd) / a;
    if (!INTERVAL_SURROUNDS(rayt, root))
    {
      return false;
    }
  }

  rec->t   = root;
  rec->p   = rayAt(ray, root);
  rec->mat = sphere->mat;

  Vec3f32 res;
  subVec3f32(&res, &rec->p, &sphere->center);

  Vec3f32 outwardNormal;
  scaleVec3f32(&outwardNormal, &res, 1 / sphere->radius);

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
    if (hitSphere(&obj.sphere, ray, CREATE_INTERVAL(rayt.min, closestHit), rec))
    {
      hit        = true;
      closestHit = rec->t;
    }
  }
  return hit;
}
