#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "ray.h"
#include "vector.h"

struct Camera
{
  f32   aspectRatio;
  i32   imageWidth;
  i32   imageHeight;
  Point center;
  Point pixel00Loc;
  Vec3  pixelDeltaU;
  Vec3  pixelDeltaV;
};
typedef struct Camera Camera;

void                  render(Camera* camera, Hittable* world, i32 worldLen);
void                  initializeCamera(Camera* camera);
Color                 rayColor(Ray* r, Hittable* world, i32 worldLength);

#endif
