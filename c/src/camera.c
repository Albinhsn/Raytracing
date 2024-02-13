#include "camera.h"
#include <stdio.h>

void render(Camera* camera, Hittable* world, i32 worldLen)
{
  initializeCamera(camera);

  printf("P3\n%d %d\n255\n", camera->imageWidth, camera->imageHeight);

  for (i32 y = 0; y < camera->imageHeight; y++)
  {
    fprintf(stderr, "\rScanlines remaining: %d", (camera->imageHeight - y));
    for (i32 x = 0; x < camera->imageWidth; x++)
    {
      Point pixelCenter  = addVec3f32(addVec3f32(camera->pixel00Loc, scaleVec3f32(camera->pixelDeltaU, x)), scaleVec3f32(camera->pixelDeltaV, y));
      Vec3  rayDirection = subVec3f32(pixelCenter, camera->center);
      Ray   r            = {.orig = camera->center, .dir = rayDirection};
      Color pixelColor   = rayColor(&r, world, worldLen);
      writeColor(pixelColor);
    }
  }
  fprintf(stderr, "\rDone                                 \n");
}
void initializeCamera(struct Camera* camera)
{
  camera->imageHeight    = camera->imageWidth / camera->aspectRatio;
  camera->imageHeight    = camera->imageHeight < 1 ? 1 : camera->imageHeight;

  f32 focalLength        = 1.0f;
  f32 viewportHeight     = 2.0f;
  f32 viewportWidth      = viewportHeight * ((f32)camera->imageWidth / camera->imageHeight);
  camera->center         = CREATE_VEC3f32(0.0f, 0.0f, 0.0f);

  Vec3 viewportU         = CREATE_VEC3f32(viewportWidth, 0.0f, 0.0f);
  Vec3 viewportV         = CREATE_VEC3f32(0.0f, -viewportHeight, 0.0f);

  camera->pixelDeltaU    = divideVec3f32(viewportU, camera->imageWidth);
  camera->pixelDeltaV    = divideVec3f32(viewportV, camera->imageHeight);

  Vec3 viewportUpperLeft = subVec3f32(subVec3f32(subVec3f32(camera->center, CREATE_VEC3f32(0.0f, 0.0f, focalLength)), divideVec3f32(viewportU, 2.0f)), divideVec3f32(viewportV, 2.0f));

  camera->pixel00Loc     = addVec3f32(viewportUpperLeft, scaleVec3f32(addVec3f32(camera->pixelDeltaU, camera->pixelDeltaV), 0.5f));
}
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
