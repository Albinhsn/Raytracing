#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "onb.h"
#include "vec3.h"

class sphere : public hittable {
public:
  // Stationary Sphere
  sphere(point3 _center1, double _radius, std::shared_ptr<material> _material)
      : center1(_center1), radius(_radius), mat(_material), is_moving(false) {
    auto rvec = vec3(radius, radius, radius);
    bbox = aabb(center1 - rvec, center1 + rvec);
  }

  // Moving Sphere
  sphere(point3 _center1, point3 _center2, double _radius,
         std::shared_ptr<material> _material)
      : center1(_center1), radius(_radius), mat(_material), is_moving(true) {

    auto rvec = vec3(radius, radius, radius);
    aabb box1(_center1 - rvec, _center1 + rvec);
    aabb box2(_center2 - rvec, _center2 + rvec);
    bbox = aabb(box1, box2);

    center_vec = _center2 - _center1;
  }
  double pdf_value(const point3 &origin, const vec3 &direction) const override {
    // This method only works for stationary spheres.

    hit_record rec;
    if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
      return 0;

    auto cos_theta_max =
        sqrt(1 - radius * radius / (center1 - origin).length_squared());
    auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return 1 / solid_angle;
  }

  vec3 random(const point3 &origin) const override {
    vec3 direction = center1 - origin;
    auto distance_squared = direction.length_squared();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
  }

  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    point3 center = is_moving ? this->center(r.time()) : center1;
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
      return false;
    }

    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (-half_b + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat;

    return true;
  }

  aabb bounding_box() const override { return bbox; }

private:
  point3 center1;
  double radius;
  shared_ptr<material> mat;
  bool is_moving;
  point3 center_vec;
  aabb bbox;

  point3 center(double time) const { return center1 + time * center_vec; }

  static void get_sphere_uv(const point3 &p, double &u, double &v) {
    auto theta = acos(-p.y());
    auto phi = atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
  }

  static vec3 random_to_sphere(double radius, double distance_squared) {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

    auto phi = 2 * pi * r1;
    auto x = cos(phi) * sqrt(1 - z * z);
    auto y = sin(phi) * sqrt(1 - z * z);

    return vec3(x, y, z);
  }
};

#endif
