// Raxel Ortiz
// CPSC 120-05
// 2021-05-06
// raxelortiz7@csu.fullerton.edu
// @raxelortiz7
//
// Lab 12-01
//
// This program generates a bunch of smooth balls!
//

#include "ray.h"

// See the header file for documentation.

Point3 Ray::origin() const { return origin_; }

Vec3 Ray::direction() const { return direction_; }

Point3 Ray::at(double t) const { return origin_ + (t * direction_); }

std::ostream& operator<<(std::ostream& out, const Ray& r) {
  out << "Ray(origin=" << r.origin() << ", direction=" << r.direction() << ")";
  return out;
}
