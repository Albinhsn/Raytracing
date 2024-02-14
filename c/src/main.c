

#include "camera.h"
#include "common.h"
#include "hittable.h"
#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

i32 main()
{

  srand(time(NULL));

  Material ground = {.type = LAMBERTIAN, .lamb = (Lambertian){.albedo = CREATE_VEC3f32(0.8f, 0.8f, 0.0f)}};
  Material center = {.type = LAMBERTIAN, .lamb = (Lambertian){.albedo = CREATE_VEC3f32(0.1f, 0.2f, 0.5f)}};
  Material left   = {.type = DIELECTRIC, .dielectric = (Dielectric){.ir = 1.5f}};
  Material right  = {
       .type = METAL, .metal = (Metal){.albedo = CREATE_VEC3f32(0.8f, 0.6f, 0.2f), .fuzz = 0.0f}
  };

  const i32 worldLen = 5;
  Hittable  world[worldLen];
  world[0] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 100.0f, .center = CREATE_VEC3f32(0, -100.5, -1), .mat = ground}
  };
  world[1] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 0.5f, .center = CREATE_VEC3f32(0, 0, -1), .mat = center}
  };
  world[2] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 0.5f, .center = CREATE_VEC3f32(-1, 0, -1), .mat = left}
  };
  world[3] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = -0.4f, .center = CREATE_VEC3f32(-1, 0, -1), .mat = left}
  };
  world[4] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 0.5f, .center = CREATE_VEC3f32(1, 0, -1), .mat = right}
  };

  Camera camera;
  camera.aspectRatio = 16.0f / 9.0f;
  camera.imageWidth  = 400;
  camera.samples     = 10;

  render(&camera, world, worldLen);
}
