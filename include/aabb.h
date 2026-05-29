#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
  public:
    interval x, y, z;

    aabb() {}

    aabb(const interval& x, const interval& y, const interval& z)
      : x(x), y(y), z(z) {
        pad_to_minimums();
    }

    aabb(const point3& a, const point3& b) {
        x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
        pad_to_minimums();
    }

    aabb(const aabb& box0, const aabb& box1) {
        x = interval(fmin(box0.x.min, box1.x.min), fmax(box0.x.max, box1.x.max));
        y = interval(fmin(box0.y.min, box1.y.min), fmax(box0.y.max, box1.y.max));
        z = interval(fmin(box0.z.min, box1.z.min), fmax(box0.z.max, box1.z.max));
    }

    const interval& axis_interval(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval ray_t) const {
        const point3& ray_orig = r.origin();
        const vec3&   ray_dir  = r.direction();

        for (int axis = 0; axis < 3; axis++) {
            const interval& ax = axis_interval(axis);
            const double adinv = 1.0 / ray_dir[axis];

            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;

            if (t0 < t1) {
                if (t0 > ray_t.min) ray_t.min = t0;
                if (t1 < ray_t.max) ray_t.max = t1;
            } else {
                if (t1 > ray_t.min) ray_t.min = t1;
                if (t0 < ray_t.max) ray_t.max = t0;
            }

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }

    int longest_axis() const {
        double dx = x.max - x.min;
        double dy = y.max - y.min;
        double dz = z.max - z.min;
        if (dx > dy)
            return dx > dz ? 0 : 2;
        else
            return dy > dz ? 1 : 2;
    }

  private:
    void pad_to_minimums() {
        double delta = 0.0001;
        if (x.max - x.min < delta) { x.min -= delta/2; x.max += delta/2; }
        if (y.max - y.min < delta) { y.min -= delta/2; y.max += delta/2; }
        if (z.max - z.min < delta) { z.min -= delta/2; z.max += delta/2; }
    }
};

#endif