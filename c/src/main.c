

#include "camera.h"
#include "common.h"
#include "hittable.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

i32 main()
{

  srand(time(NULL));

  Material ground   = {.type = LAMBERTIAN, .lamb = (Lambertian){.albedo = CREATE_VEC3f32(0.5f, 0.5f, 0.5f)}};
  i32      worldLen = 1000;
  Hittable world[worldLen];
  world[0] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 1000.0f, .center = CREATE_VEC3f32(0, -1000, 0), .mat = ground}
  };

  u32 count = 1;

  for (i32 a = -11; a < 11; a++)
  {
    for (i32 b = -11; b < 11; b++)
    {
      f32     chooseMat = RANDOM_DOUBLE;
      Point   center    = (Point){.x = a + 0.9 * RANDOM_DOUBLE, 0.2, b + 0.9 * RANDOM_DOUBLE};

      Vec3f32 subRes;
      Vec3f32 idk = CREATE_VEC3f32(4.0f, 0.2, 0.0f);
      subVec3f32(&subRes, &center, &idk);
      if (lengthVec3f32(&subRes) > 0.9f)
      {
        Material sphereMaterial;

        if (chooseMat < 0.8)
        {

          Vec3  r0 = randomVec3f32();
          Vec3  r1 = randomVec3f32();

          Color albedo;
          mulVec3f32(&albedo, &r0, &r1);

          sphereMaterial.type = LAMBERTIAN;
          sphereMaterial.lamb = (Lambertian){.albedo = albedo};
        }
        else if (chooseMat < 0.95)
        {
          Vec3  r0 = randomVec3f32();
          Vec3  r1 = randomVec3f32();

          Color albedo;
          mulVec3f32(&albedo, &r0, &r1);

          f32 fuzz             = RANDOM_DOUBLE_IN_RANGE(0, 0.5);
          sphereMaterial.type  = METAL;
          sphereMaterial.metal = (Metal){.albedo = albedo, .fuzz = fuzz};
        }
        else
        {
          sphereMaterial.type       = DIELECTRIC;
          sphereMaterial.dielectric = (Dielectric){.ir = 1.5f};
        }
        world[count].sphere.mat    = sphereMaterial;
        world[count].type          = SPHERE;
        world[count].sphere.center = center;
        world[count].sphere.radius = 0.2f;

        count++;
      }
    }
  }
  Material mat1                  = (Material){.type = DIELECTRIC, .dielectric = (Dielectric){.ir = 1.5f}};
  world[count + 0].type          = SPHERE;
  world[count + 0].sphere.radius = 1.0f;
  world[count + 0].sphere.center = (Point){0, 1, 0};
  world[count + 0].sphere.mat    = mat1;

  Material mat2                  = (Material){.type = LAMBERTIAN, .lamb = (Lambertian){.albedo = (Color){0.4, 0.2, 0.1}}};
  world[count + 1].type          = SPHERE;
  world[count + 1].sphere.radius = 1.0f;
  world[count + 1].sphere.center = (Point){-4, 1, 0};
  world[count + 1].sphere.mat    = mat2;

  Material mat3                  = (Material){
                       .type = METAL, .metal = (Metal){.albedo = (Color){0.7, 0.6, 0.5}, .fuzz = 0.0}
  };
  world[count + 2].type          = SPHERE;
  world[count + 2].sphere.radius = 1.0f;
  world[count + 2].sphere.center = (Point){4, 1, 0};
  world[count + 2].sphere.mat    = mat3;
  count += 3;

  worldLen = count;

  Camera camera;
  camera.aspectRatio  = 16.0f / 9.0f;
  camera.imageWidth   = 400;
  camera.samples      = 10;
  camera.maxDepth     = 10;

  camera.vfov         = 20;
  camera.lookfrom     = (Point){13, 2, 3};
  camera.lookat       = (Point){0, 0, 0};
  camera.vUp          = (Vec3f32){0, 1, 0};

  camera.defocusAngle = 0.6f;
  camera.focusDist    = 10.0f;

  render(&camera, world, worldLen);
}
