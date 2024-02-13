

#include "camera.h"
#include "common.h"
#include "hittable.h"
#include "vector.h"
#include <stdlib.h>
#include <stdbool.h>

i32 main()
{
  srand(time(NULL));

  const i32 worldLen = 2;
  Hittable  world[worldLen];
  world[0] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 0.5f, .center = CREATE_VEC3f32(0, 0, -1)}
  };
  world[1] = (Hittable){
      .type = SPHERE, .sphere = (Sphere){.radius = 100.0f, .center = CREATE_VEC3f32(0, -100.5, -1)}
  };

  Camera camera;
  camera.aspectRatio = 16.0f / 9.0f;
  camera.imageWidth  = 400;
  camera.samples = 100;

  render(&camera, world, worldLen);
}
