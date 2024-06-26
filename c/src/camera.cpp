#include "camera.h"
#include "common.h"
#include "hittable.h"
#include "vector.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void pixelSampleSquare(Vec3f32* res, Vec3f32* pixelDeltaU, Vec3f32* pixelDeltaV)
{
  f32     px = -0.5f + RANDOM_DOUBLE;
  f32     py = -0.5f + RANDOM_DOUBLE;

  Vec3f32 scaledU;
  scaleVec3f32(&scaledU, pixelDeltaU, px);

  Vec3f32 scaledV;
  scaleVec3f32(&scaledV, pixelDeltaV, py);

  addVec3f32(res, &scaledU, &scaledV);
}

Point defocusDiskSample(Camera* camera)
{
  Point   p = randomVec3f32InUnitDisk();
  Vec3f32 scaledDiskU;
  scaleVec3f32(&scaledDiskU, &camera->defocusDiskU, p.x);

  Vec3f32 scaledDiskV;
  scaleVec3f32(&scaledDiskV, &camera->defocusDiskV, p.y);

  Vec3f32 diskSum;
  addVec3f32(&diskSum, &scaledDiskU, &scaledDiskV);

  Vec3f32 res;
  addVec3f32(&res, &camera->center, &diskSum);
  return res;
}

Ray getRay(Camera* camera, i32 x, i32 y)
{
  Vec3f32 scaledDeltaV;
  scaleVec3f32(&scaledDeltaV, &camera->pixelDeltaV, y);

  Vec3f32 scaledDeltaU;
  scaleVec3f32(&scaledDeltaU, &camera->pixelDeltaU, x);

  Vec3f32 pixelDelta;
  addVec3f32(&pixelDelta, &camera->pixel00Loc, &scaledDeltaU);

  Point pixelCenter;
  addVec3f32(&pixelCenter, &pixelDelta, &scaledDeltaV);

  Point   pixelSample;
  Vec3f32 deltaSquared;
  pixelSampleSquare(&deltaSquared, &camera->pixelDeltaU, &camera->pixelDeltaV);

  addVec3f32(&pixelSample, &pixelCenter, &deltaSquared);

  Vec3f32 rayOrigin = camera->defocusAngle <= 0 ? camera->center : defocusDiskSample(camera);
  Vec3f32 rayDirection;
  subVec3f32(&rayDirection, &pixelSample, &rayOrigin);

  return (Ray){.orig = rayOrigin, .dir = rayDirection};
}

struct GatherColorArgs
{
  u32       samples;
  u32       minY;
  u32       maxY;
  Color*    res;
  Camera*   camera;
  Hittable* world;
  u32       worldLen;
};
typedef struct GatherColorArgs GatherColorArgs;

void*                          gatherColors(void* arg)
{
  GatherColorArgs args   = *(GatherColorArgs*)arg;
  Camera*          camera = args.camera;
  u32              width  = camera->imageWidth;

  u32              minY = args.minY, maxY = args.maxY;
  u32              samples = args.samples;
  for (u32 y = minY; y < maxY; y++)
  {
    for (u32 x = 0; x < width; x++)
    {
      u32 idx = y * width + x;
      for (u32 sample = 0; sample < samples; sample++)
      {
        Ray     r           = getRay(camera, x, y);
        Vec3f32 newRayColor = rayColor(&r, camera->maxDepth, args.world, args.worldLen);
        Color   a           = args.res[idx];
        addVec3f32(&args.res[idx], &a, &newRayColor);
      }
    }
  }
  return 0;
}

#define nmbrOfThreads 10

void render(Camera* camera, Hittable* world, i32 worldLen)
{
  initializeCamera(camera);

  FILE* filePtr = fopen("./image.ppm", "w");
  fprintf(filePtr, "P3\n%d %d\n255\n", camera->imageWidth, camera->imageHeight);

  u32   samples = camera->samples;
  u32   yDiff   = camera->imageHeight / nmbrOfThreads;

  Color image[camera->imageWidth * camera->imageHeight];

  fprintf(stderr, "STARTING\n");

  pthread_t       threadIds[nmbrOfThreads];
  GatherColorArgs args[nmbrOfThreads];
  for (i32 i = 0; i < nmbrOfThreads; i++)
  {
    args[i].camera   = camera;
    args[i].samples  = samples;
    args[i].minY     = yDiff * i;
    args[i].maxY     = yDiff * (i + 1);
    args[i].world    = world;
    args[i].worldLen = worldLen;
    args[i].res      = &image[0];

    threadIds[i]     = i;
  }

  for (i32 i = 0; i < nmbrOfThreads; i++)
  {
    pthread_create(&threadIds[i], NULL, gatherColors, (void*)&args[i]);
  }

  for (i32 i = 0; i < nmbrOfThreads; i++)
  {
    if (pthread_join(threadIds[i], NULL) != 0)
    {
      printf("Failed to join?\n");
      exit(2);
    }
  }
  f32 finalSamples = camera->samples;
  i32 tot          = camera->imageWidth * camera->imageHeight;
  for (i32 i = 0; i < tot; i++)
  {
    Vec3f32  v        = image[i];
    f32      scale    = 1.0 / finalSamples;
    f32      r        = LINEAR_TO_GAMMA((v.x * scale));
    f32      g        = LINEAR_TO_GAMMA((v.y * scale));
    f32      b        = LINEAR_TO_GAMMA((v.z * scale));

    Interval interval = CREATE_INTERVAL(0.0, 0.999f);

    fprintf(filePtr, "%d %d %d\n", (i32)(256 * clamp(interval, r)), (i32)(256 * clamp(interval, g)), (i32)(256 * clamp(interval, b)));
  }
}
void initializeCamera(struct Camera* camera)
{
  camera->imageHeight    = camera->imageWidth / camera->aspectRatio;
  camera->imageHeight    = camera->imageHeight < 1 ? 1 : camera->imageHeight;

  camera->center         = camera->lookfrom;

  f32     theta          = DEGREES_TO_RADIANS(camera->vfov);
  f32     h              = tan(theta / 2);
  f32     viewportHeight = 2.0f * h * camera->focusDist;
  f32     viewportWidth  = viewportHeight * ((f32)camera->imageWidth / camera->imageHeight);

  Vec3f32 res;
  subVec3f32(&res, &camera->lookfrom, &camera->lookat);
  normalizeVec3f32(&camera->w, &res);

  Vec3f32 crossCam;
  crossVec3f32(&crossCam, &camera->vUp, &camera->w);
  normalizeVec3f32(&camera->u, &crossCam);

  crossVec3f32(&camera->v, &camera->w, &camera->u);

  Vec3f32 viewportU;
  scaleVec3f32(&viewportU, &camera->u, viewportWidth);

  Vec3f32 viewportV;
  Vec3f32 negCamV = NEG_VEC3f32(camera->v);
  scaleVec3f32(&viewportV, &negCamV, viewportHeight);

  scaleVec3f32(&camera->pixelDeltaU, &viewportU, 1.0 / (f32)camera->imageWidth);
  scaleVec3f32(&camera->pixelDeltaV, &viewportV, 1.0 / (f32)camera->imageHeight);

  Vec3f32 scaledViewportV;
  scaleVec3f32(&scaledViewportV, &viewportV, 1.0f / 2.0f);

  Vec3f32 scaledViewportU;
  scaleVec3f32(&scaledViewportU, &viewportU, 1.0f / 2.0f);

  Vec3f32 scaledCamW;
  scaleVec3f32(&scaledCamW, &camera->w, camera->focusDist);

  Vec3f32 v0;
  subVec3f32(&v0, &camera->center, &scaledCamW);

  Vec3f32 v1;
  subVec3f32(&v1, &v0, &scaledViewportU);

  Vec3f32 viewportUpperLeft;
  subVec3f32(&viewportUpperLeft, &v1, &scaledViewportV);

  Vec3f32 camSum;
  addVec3f32(&camSum, &camera->pixelDeltaU, &camera->pixelDeltaV);

  Vec3f32 scaledCamSum;
  scaleVec3f32(&scaledCamSum, &camSum, 0.5f);
  addVec3f32(&camera->pixel00Loc, &viewportUpperLeft, &scaledCamSum);

  f32 defocusRadius = camera->focusDist * tan(DEGREES_TO_RADIANS(camera->defocusAngle / 2));

  scaleVec3f32(&camera->defocusDiskU, &camera->u, defocusRadius);
  scaleVec3f32(&camera->defocusDiskV, &camera->v, defocusRadius);
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
    default:
    {
      scatter = false;
    }
    }
    if (scatter)
    {
      Vec3f32 res;
      Vec3f32 rayC = rayColor(&scattered, depth - 1, world, worldLength);
      mulVec3f32(&res, &attenuation, &rayC);
      return res;
    }
    return BLACK;
  }

  Vec3f32 unitDirection;
  normalizeVec3f32(&unitDirection, &r->dir);

  f32     a = 0.5 * (unitDirection.y + 1.0f);

  Vec3f32 backgroundColor;
  Vec3f32 someBlue = SOMEBLUE;
  scaleVec3f32(&backgroundColor, &someBlue, a);

  Vec3f32 intColor;
  Vec3f32 white = WHITE;
  scaleVec3f32(&intColor, &white, (1.0 - a));
  Vec3f32 res;
  addVec3f32(&res, &intColor, &backgroundColor);

  return res;
}
