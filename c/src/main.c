

#include "common.h"
#include "hittable.h"
#include "ray.h"
#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

Color rayColor(Ray* r, Hittable* world, i32 worldLength)
{
  HitRecord rec;
  if (calculateRayIntersection(world, worldLength, r, CREATE_INTERVAL(0, INFINITY), &rec))
  {
    return scaleVec3f32(addVec3f32(rec.normal, CREATE_VEC3f32(1.0f, 1.0f, 1.0f)), 0.5f);
  }

  Vec3 unitDirection = normalizeVec3f32(r->dir);
  f32  a             = 0.5 * (unitDirection.y + 1.0f);
  return addVec3f32(scaleVec3f32(WHITE, (1.0 - a)), scaleVec3f32(SOMEBLUE, a));
}

i32 main()
{
  f32 aspectRatio    = 16.0f / 9.0f;
  i32 imageWidth     = 400;
  i32 imageHeight    = imageWidth / aspectRatio;
  imageHeight        = imageHeight < 1 ? 1 : imageHeight;

  const i32 worldLen = 2;
  Hittable  world[worldLen];
  world[0] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 0.5f, .center = CREATE_VEC3f32(0, 0, -1)}
  };
  world[1] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 100.0f, .center = CREATE_VEC3f32(0, -100.5, -1)}
  };

  f32   focalLength       = 1.0f;
  f32   viewportHeight    = 2.0f;
  f32   viewportWidth     = viewportHeight * ((f32)imageWidth / imageHeight);
  Point cameraCenter      = CREATE_VEC3f32(0.0f, 0.0f, 0.0f);

  Vec3  viewportU         = CREATE_VEC3f32(viewportWidth, 0.0f, 0.0f);
  Vec3  viewportV         = CREATE_VEC3f32(0.0f, -viewportHeight, 0.0f);

  Vec3  pixelDeltaU       = divideVec3f32(viewportU, imageWidth);
  Vec3  pixelDeltaV       = divideVec3f32(viewportV, imageHeight);

  Vec3  viewportUpperLeft = subVec3f32(subVec3f32(subVec3f32(cameraCenter, CREATE_VEC3f32(0.0f, 0.0f, focalLength)), divideVec3f32(viewportU, 2.0f)), divideVec3f32(viewportV, 2.0f));

  Point pixel00Loc        = addVec3f32(viewportUpperLeft, scaleVec3f32(addVec3f32(pixelDeltaU, pixelDeltaV), 0.5f));

  printf("P3\n%d %d\n255\n", imageWidth, imageHeight);

  for (i32 y = 0; y < imageHeight; y++)
  {
    fprintf(stderr, "\rScanlines remaining: %d", (imageHeight - y));
    for (i32 x = 0; x < imageWidth; x++)
    {
      Point pixelCenter  = addVec3f32(addVec3f32(pixel00Loc, scaleVec3f32(pixelDeltaU, x)), scaleVec3f32(pixelDeltaV, y));
      Vec3  rayDirection = subVec3f32(pixelCenter, cameraCenter);
      Ray   r            = {.orig = cameraCenter, .dir = rayDirection};
      Color pixelColor   = rayColor(&r, world, worldLen);
      writeColor(pixelColor);
    }
  }
  fprintf(stderr, "\rDone                                 \n");
}
