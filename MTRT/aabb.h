#pragma once
#include "vec3.h"
#include "ray.h"
#include <algorithm>
inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }
class Aabb {
public:
	Aabb() {}
	Aabb(const Vec3& min, const Vec3& max) :max_(max), min_(min) {}
	Vec3 GetMinCorner() const { return min_; }
	Vec3 GetMaxCorner() const { return max_; }
	bool Hit(const Ray& r, float tmin, float tmax)const {
		for (int i = 0; i < 3; i++) {
			float inv_direc_comp = 1.0f / r.GetDirection()[i];
			float t0 = (min_[i] - r.GetOrigin()[i]) * inv_direc_comp;
			float t1 = (max_[i] - r.GetOrigin()[i]) * inv_direc_comp;
			if (inv_direc_comp < 0.0f)
				std::swap(t0, t1);
			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			if (tmax <= tmin)
				return false;
		}
		return true;
	}
	Vec3 min_, max_;
};

Aabb MergeTwoBoundingBoxes(const Aabb& box0, const Aabb& box1) {
	Vec3 new_min(ffmin(box0.GetMinCorner().x(), box1.GetMinCorner().x()),
			   ffmin(box0.GetMinCorner().y(), box1.GetMinCorner().y()),
			   ffmin(box0.GetMinCorner().z(), box1.GetMinCorner().z()));

	Vec3 new_max(ffmax(box0.GetMaxCorner().x(), box1.GetMaxCorner().x()),
				 ffmax(box0.GetMaxCorner().y(), box1.GetMaxCorner().y()),
				 ffmax(box0.GetMaxCorner().z(), box1.GetMaxCorner().z()));
	return Aabb(new_min, new_max);
}

