#pragma once
#include "hitable.h"
#include "ray.h"
class XYRect: public Hitable {
public:
	XYRect() {}
	~XYRect() {
	}
	XYRect(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> mat_ptr) :x0_(x0), x1_(x1), y0_(y0), y1_(y1),k_(k),mat_ptr_(mat_ptr) {};
	virtual bool Hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const {
		box = Aabb(Vec3(x0_, y0_, k_ - FLOAT_EPSILON), Vec3(x1_, y1_, k_ + FLOAT_EPSILON));
		return true;
	}
	std::shared_ptr<Material> mat_ptr_;
	float x0_,x1_,y0_,y1_,k_;
};

bool XYRect::Hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
	float t = (k_ - r.GetOrigin().z()) / r.GetDirection().z();
	if (t < t0 || t > t1)
		return false;
	float x = r.GetOrigin().x() + t * r.GetDirection().x();
	if (x < x0_ || x > x1_)
		return false;
	float y = r.GetOrigin().y() + t * r.GetDirection().y();
	if (y < y0_ || y > y1_)
		return false;
	rec.u = (x - x0_) / (x1_ - x0_);
	rec.v = (y - y0_) / (y1_ - y0_);
	rec.t = t;
	rec.mat_ptr = mat_ptr_;
	rec.pos = r.GetPointAtParameter(t);
	rec.normal = Vec3(0, 0, 1);
	return true;

}