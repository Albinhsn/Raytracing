#include "vector.h"
#include "common.h"
#include <math.h>
#include <stdio.h>

Vec3f32 randomVec3f32()
{
  return (Vec3f32){RANDOM_DOUBLE, RANDOM_DOUBLE, RANDOM_DOUBLE};
}
Vec3f32 randomVec3f32InRange(f32 min, f32 max)
{
  return (Vec3f32){
      RANDOM_DOUBLE_IN_RANGE(min, max), //
      RANDOM_DOUBLE_IN_RANGE(min, max), //
      RANDOM_DOUBLE_IN_RANGE(min, max)  //
  };
}

Vec3f32 randomVec3f32InUnitSphere()
{
  while (true)
  {
    Vec3 p = randomVec3f32InRange(-1, 1);
    if (lengthSquaredVec3f32(p) < 1)
    {
      return p;
    }
  }
}
Vec3f32 randomUnitVector()
{
  return normalizeVec3f32(randomVec3f32InUnitSphere());
}

Vec3f32 randomVec3f32OnHemisphere(Vec3f32 normal)
{
  Vec3f32 unitSphereVec = randomUnitVector();
  if (dotVec3f32(unitSphereVec, normal) > 0.0)
  {
    return unitSphereVec;
  }
  else
  {
    return CREATE_VEC3f32(-unitSphereVec.x, -unitSphereVec.y, -unitSphereVec.z);
  }
}
f32 reflectanceVec3f32(f32 cosine, f32 refIdx)
{
  f32 r0 = (1 - refIdx) / (1 + refIdx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow(1 - cosine, 5);
}

bool intervalContains(Interval i, f32 x)
{
  return i.min <= x && x <= i.max;
}
bool intervalSurrounds(Interval i, f32 x)
{
  return i.min < x && x < i.max;
}

f32 clamp(Interval i, f32 x)
{
  if (x < i.min)
  {
    return i.min;
  }
  if (x > i.max)
  {
    return i.max;
  }
  return x;
}

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
f32 dotVec3f32(Vec3f32 v0, Vec3f32 v1)
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

Vec3f32 refractVec3f32(Vec3f32 uv, Vec3f32 n, f32 etaiOverEtat)
{
  f32     cosTheta     = fmin(dotVec3f32(CREATE_VEC3f32(-uv.x, -uv.y, -uv.z), n), 1.0f);

  Vec3f32 rOutPerp     = scaleVec3f32(addVec3f32(uv, scaleVec3f32(n, cosTheta)), etaiOverEtat);
  Vec3f32 rOutParallel = scaleVec3f32(n, -sqrt(fabs(1.0 - lengthSquaredVec3f32(rOutPerp))));
  Vec3f32 f            = addVec3f32(rOutPerp, rOutParallel);
  return f;
}

Vec3f32 reflectVec3f32(Vec3f32 v, Vec3f32 n)
{
  return subVec3f32(v, scaleVec3f32(n, 2 * dotVec3f32(v, n)));
}

bool nearZeroVec3f32(Vec3f32 v)
{
  f32 s = 1e-8;
  return fabs(v.x) < s && fabs(v.y) < s && fabs(v.z) < s;
}

void writeColor(Color v, i32 samples)
{
  f32 r     = v.x;
  f32 g     = v.y;
  f32 b     = v.z;

  f32 scale = 1.0 / (f32)samples;
  r *= scale;
  g *= scale;
  b *= scale;

  r          = LINEAR_TO_GAMMA(r);
  g          = LINEAR_TO_GAMMA(g);
  b          = LINEAR_TO_GAMMA(b);

  Interval i = CREATE_INTERVAL(0.0, 0.999f);

  printf("%d %d %d\n", (i32)(256 * clamp(i, r)), (i32)(256 * clamp(i, g)), (i32)(256 * clamp(i, b)));
}

void debugVec3f32(Vec3f32 v)
{
  printf("%lf %lf %lf\n", v.x, v.y, v.z);
}
