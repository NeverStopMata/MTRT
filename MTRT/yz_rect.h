#pragma once
#include "hitable.h"
#include "ray.h"
class YZRect : public Hitable {
public:
	YZRect() {}
	YZRect(float y0, float y1, float z0, float z1, float k, Material * mat_ptr) :y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), mat_ptr_(mat_ptr) {};
	virtual bool Hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const {
		box = Aabb(Vec3(k_ - FLOAT_EPSILON, y0_, z0_), Vec3(k_ + FLOAT_EPSILON, y1_, z1_));
		return true;
	}
	Material* mat_ptr_;
	float y0_, y1_, z0_, z1_, k_;
};

bool YZRect::Hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
	float t = (k_ - r.GetOrigin().x()) / r.GetDirection().x();
	if (t < t0 || t > t1)
		return false;
	float y = r.GetOrigin().y() + t * r.GetDirection().y();
	if (y < y0_ || y > y1_)
		return false;
	float z = r.GetOrigin().z() + t * r.GetDirection().z();
	if (z < z0_ || z > z1_)
		return false;
	rec.u = (y - y0_) / (y1_ - y0_);
	rec.v = (z - z0_) / (z1_ - z0_);
	rec.t = t;
	rec.mat_ptr = mat_ptr_;
	rec.pos = r.GetPointAtParameter(t);
	rec.normal = Vec3(1, 0, 0);
	return true;
}