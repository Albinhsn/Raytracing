#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vector.h"
#include <stdbool.h>

struct HitRecord
{
  Point p;
  Vec3  normal;
  f32   t;
  bool  frontFace;
};
typedef struct HitRecord HitRecord;

struct Sphere
{
  Point center;
  f32   radius;
};
typedef struct Sphere    Sphere;

typedef struct HitRecord HitRecord;

enum HittableTypes
{
  SPHERE
};
typedef enum HittableTypes HittableTypes;

struct Hittable
{
  HittableTypes type;
  union
  {
    Sphere sphere;
  };
};
typedef struct Hittable Hittable;

bool                    hitSphere(Sphere* sphere, Ray* ray, Interval rayt, HitRecord* rec);
bool                    calculateRayIntersection(Hittable* hittableObjects, i32 length, Ray* ray, Interval rayt, HitRecord* rec);

#endif
