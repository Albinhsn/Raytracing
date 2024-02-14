#include "camera.h"
#include "common.h"
#include "hittable.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>

Vec3 pixelSampleSquare(Vec3 pixelDeltaU, Vec3 pixelDeltaV)
{
  f32 px = -0.5f + RANDOM_DOUBLE;
  f32 py = -0.5f + RANDOM_DOUBLE;

  return addVec3f32(scaleVec3f32(pixelDeltaU, px), scaleVec3f32(pixelDeltaV, py));
}

Point defocusDiskSample(Camera* camera)
{
  Point p = randomVec3f32InUnitDisk();
  p       = (Vec3){-0.184263, -0.528258, 0.0};
  return addVec3f32(camera->center, addVec3f32(scaleVec3f32(camera->defocusDiskU, p.x), scaleVec3f32(camera->defocusDiskV, p.y)));
}

Ray getRay(Camera* camera, i32 x, i32 y)
{
  Point   pixelCenter  = addVec3f32(addVec3f32(camera->pixel00Loc, scaleVec3f32(camera->pixelDeltaU, x)), scaleVec3f32(camera->pixelDeltaV, y));
  Point   pixelSample  = addVec3f32(pixelCenter, pixelSampleSquare(camera->pixelDeltaU, camera->pixelDeltaV));

  Vec3f32 rayOrigin    = camera->defocusAngle <= 0 ? camera->center : defocusDiskSample(camera);
  Vec3    rayDirection = subVec3f32(pixelSample, rayOrigin);

  return (Ray){.orig = rayOrigin, .dir = rayDirection};
}

void render(Camera* camera, Hittable* world, i32 worldLen)
{
  initializeCamera(camera);

  printf("P3\n%d %d\n255\n", camera->imageWidth, camera->imageHeight);

  for (i32 y = 0; y < camera->imageHeight; y++)
  {
    fprintf(stderr, "\rScanlines remaining: %d   ", (camera->imageHeight - y));
    for (i32 x = 0; x < camera->imageWidth; x++)
    {
      Color pixelColor = BLACK;
      for (i32 sample = 0; sample < camera->samples; sample++)
      {
        Ray r      = getRay(camera, x, y);
        pixelColor = addVec3f32(pixelColor, rayColor(&r, camera->maxDepth, world, worldLen));
      }
      writeColor(pixelColor, camera->samples);
    }
  }
  fprintf(stderr, "\rDone                                 \n");
}
void initializeCamera(struct Camera* camera)
{
  camera->imageHeight    = camera->imageWidth / camera->aspectRatio;
  camera->imageHeight    = camera->imageHeight < 1 ? 1 : camera->imageHeight;

  camera->center         = camera->lookfrom;

  f32 theta              = DEGREES_TO_RADIANS(camera->vfov);
  f32 h                  = tan(theta / 2);
  f32 viewportHeight     = 2.0f * h * camera->focusDist;
  f32 viewportWidth      = viewportHeight * ((f32)camera->imageWidth / camera->imageHeight);

  camera->w              = normalizeVec3f32(subVec3f32(camera->lookfrom, camera->lookat));
  camera->u              = normalizeVec3f32(crossVec3f32(camera->vUp, camera->w));
  camera->v              = crossVec3f32(camera->w, camera->u);

  Vec3 viewportU         = scaleVec3f32(camera->u, viewportWidth);
  Vec3 viewportV         = scaleVec3f32(CREATE_VEC3f32(-camera->v.x, -camera->v.y, -camera->v.z), viewportHeight);

  camera->pixelDeltaU    = divideVec3f32(viewportU, camera->imageWidth);
  camera->pixelDeltaV    = divideVec3f32(viewportV, camera->imageHeight);

  Vec3 viewportUpperLeft = subVec3f32(subVec3f32(subVec3f32(camera->center, scaleVec3f32(camera->w, camera->focusDist)), divideVec3f32(viewportU, 2.0f)), divideVec3f32(viewportV, 2.0f));

  camera->pixel00Loc     = addVec3f32(viewportUpperLeft, scaleVec3f32(addVec3f32(camera->pixelDeltaU, camera->pixelDeltaV), 0.5f));

  f32 defocusRadius      = camera->focusDist * tan(DEGREES_TO_RADIANS(camera->defocusAngle / 2));

  camera->defocusDiskU   = scaleVec3f32(camera->u, defocusRadius);
  camera->defocusDiskV   = scaleVec3f32(camera->v, defocusRadius);
}
Color rayColor(Ray* r, i32 depth, Hittable* world, i32 worldLength)
{
  if (depth <= 0)
  {
    return BLACK;
  }
  HitRecord rec;
  if (calculateRayIntersection(world, worldLength, r, CREATE_INTERVAL(0.01, INFINITY), &rec))
  {
    Ray   scattered;
    Color attenuation;
    bool  scatter;
    switch (rec.mat.type)
    {
    case LAMBERTIAN:
    {
      scatter = scatterLambertian(rec.mat.lamb, &rec, &attenuation, &scattered);
      break;
    }
    case METAL:
    {
      scatter = scatterMetal(rec.mat.metal, r, &rec, &attenuation, &scattered);
      break;
    }
    case DIELECTRIC:
    {
      scatter = scatterDielectric(rec.mat.dielectric, r, &rec, &attenuation, &scattered);
      break;
    }
    }
    if (scatter)
    {
      return mulVec3f32(attenuation, rayColor(&scattered, depth - 1, world, worldLength));
    }
    return BLACK;
  }

  Vec3 unitDirection = normalizeVec3f32(r->dir);
  f32  a             = 0.5 * (unitDirection.y + 1.0f);
  return addVec3f32(scaleVec3f32(WHITE, (1.0 - a)), scaleVec3f32(SOMEBLUE, a));
}
