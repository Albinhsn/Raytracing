#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "ray.h"
#include "vector.h"

struct Camera
{
  f32     aspectRatio;
  i32     imageWidth;
  i32     imageHeight;
  i32     maxDepth;
  Point   center;
  Point   pixel00Loc;
  Vec3    pixelDeltaU;
  Vec3    pixelDeltaV;
  u32     samples;
  f32     vfov;
  Point   lookfrom;
  Point   lookat;
  Vec3f32 vUp;
  Vec3    u, v, w;
};
typedef struct Camera Camera;

void                  render(Camera* camera, Hittable* world, i32 worldLen);
void                  initializeCamera(Camera* camera);
Color                 rayColor(Ray* r, i32 maxDepth, Hittable* world, i32 worldLength);

#endif
