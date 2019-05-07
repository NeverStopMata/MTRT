#pragma once
#include "hitable.h"
class Translate : public Hitable {
public:
	Translate(Hitable * hitable_ptr, const Vec3& displacement) :hitable_ptr_(hitable_ptr),offset_(displacement){}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const;
	Hitable * hitable_ptr_;
	Vec3 offset_;
};

bool Translate::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	Ray moved_r(r.GetOrigin() - offset_, r.GetDirection(), r.GetTime());
	if (hitable_ptr_->Hit(moved_r, t_min, t_max, rec)) {
		rec.pos += offset_;
		return true;
	}
	else {
		return false;
	}
}

bool Translate::GetBoundingBox(float t0, float t1, Aabb& box) const {
	if (hitable_ptr_->GetBoundingBox(t0, t1, box)) {
		box = Aabb(box.GetMinCorner() + offset_, box.GetMaxCorner() + offset_);
		return true;
	}
	else {
		return false;
	}
}
