#include "vector.h"
#include <math.h>
#include <stdio.h>

Vec3f32 scaleVec3f32(Vec3f32 v, f32 t)
{
  Vec3f32 res = {.x = v.x * t, .y = v.y * t, .z = v.z * t};
  return res;
}
Vec3f32 addVec3f32(Vec3f32 v0, Vec3f32 v1)
{
  Vec3f32 res = {
      .x = v0.x + v1.x, //
      .y = v0.y + v1.y, //
      .z = v0.z + v1.z  //
  };
  return res;
}
Vec3f32 subVec3f32(Vec3f32 v0, Vec3f32 v1)
{
  Vec3f32 res = {
      .x = v0.x - v1.x, //
      .y = v0.y - v1.y, //
      .z = v0.z - v1.z  //
  };
  return res;
}
Vec3f32 mulVec3f32(Vec3f32 v0, Vec3f32 v1)
{
  Vec3f32 res = {
      .x = v0.x * v1.x, //
      .y = v0.y * v1.y, //
      .z = v0.z * v1.z  //
  };
  return res;
}
Vec3f32 divideVec3f32(Vec3f32 v, f32 t)
{
  Vec3f32 res = {
      .x = v.x / t, //
      .y = v.y / t, //
      .z = v.z / t  //
  };
  return res;
}
f32 lengthVec3f32(Vec3f32 v)
{
  return sqrtf(lengthSquaredVec3f32(v));
}
f32 lengthSquaredVec3f32(Vec3f32 v)
{
  return v.x * v.x + v.y * v.y + v.z * v.z;
}
f32 dotProductVec3f32(Vec3f32 v0, Vec3f32 v1)
{
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}
Vec3f32 crossVec3f32(Vec3f32 v0, Vec3f32 v1)
{
  Vec3f32 res = {
      .x = v0.y * v1.z - v0.z * v1.y, //
      .y = v0.z * v1.x - v0.x * v1.z, //
      .z = v0.x * v1.y - v0.y * v1.x  //
  };
  return res;
}
Vec3f32 normalizeVec3f32(Vec3f32 v)
{
  return divideVec3f32(v, lengthVec3f32(v));
}
void writeColor(Color v)
{
  printf("%d %d %d\n", (i32)(v.x * 255.999), (i32)(v.y * 255.999), (i32)(v.z * 255.999));
}

void debugVec3f32(Vec3f32 v)
{
  printf("%lf %lf %lf\n", v.x, v.y, v.z);
}
