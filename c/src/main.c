

#include "common.h"
#include "ray.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>

bool hitSphere(Point center, f32 radius, Ray r)
{
  Vec3 oc           = subVec3f32(r.orig, center);
  f32  a            = dotVec3f32(r.dir, r.dir);
  f32  b            = 2.0f * dotVec3f32(oc, r.dir);
  f32  c            = dotVec3f32(oc, oc) - radius * radius;

  f32  discriminant = b * b - 4 * a * c;

  return discriminant >= 0;
}

Color rayColor(Ray r)
{
  if(hitSphere(CREATE_VEC3f32(0.0f, 0.0f, -1.0f), 0.5f, r)){
    return RED;
  }

  Vec3 unitDirection = normalizeVec3f32(r.dir);
  f32  a             = 0.5 * (unitDirection.y + 1.0f);
  return addVec3f32(scaleVec3f32(WHITE, (1.0 - a)), scaleVec3f32(SOMEBLUE, a));
}

i32 main()
{
  f32 aspectRatio         = 16.0f / 9.0f;
  i32 imageWidth          = 400;
  i32 imageHeight         = imageWidth / aspectRatio;
  imageHeight             = imageHeight < 1 ? 1 : imageHeight;

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
      Color pixelColor   = rayColor(r);
      writeColor(pixelColor);
    }
  }
  fprintf(stderr, "\rDone                                 \n");
}
