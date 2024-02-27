#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vector.h"
#include <stdbool.h>

struct Dielectric
{
  f32 ir;
};
typedef struct Dielectric Dielectric;

struct Lambertian
{
  Color albedo;
};
typedef struct Lambertian Lambertian;

struct Metal
{
  Color albedo;
  f32   fuzz;
};
typedef struct Metal Metal;

enum MaterialTypes
{
  LAMBERTIAN,
  METAL,
  DIELECTRIC
};
typedef enum MaterialTypes MaterialTypes;

struct Material
{
  MaterialTypes type;
  union
  {
    Lambertian lamb;
    Metal      metal;
    Dielectric dielectric;
  };
};
typedef struct Material Material;

struct HitRecord
{
  Point    p;
  Vec3f32  normal;
  Material mat;
  f32      t;
  bool     frontFace;
};
typedef struct HitRecord HitRecord;

struct Sphere
{
  Point    center;
  f32      radius;
  Material mat;
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

bool                    scatterMetal(Metal metal, Ray* rayIn, HitRecord* rec, Color* attenuation, Ray* scattered);
bool                    scatterLambertian(Lambertian lambertian, HitRecord* rec, Color* attenuation, Ray* scattered);
bool                    scatterDielectric(Dielectric dielectric, Ray* rayIn, HitRecord* rec, Color* attenuation, Ray* scattered);
bool                    hitSphere(Sphere* sphere, Ray* ray, Interval rayt, HitRecord* rec);
bool                    calculateRayIntersection(Hittable* hittableObjects, i32 length, Ray* ray, Interval rayt, HitRecord* rec);

#endif
