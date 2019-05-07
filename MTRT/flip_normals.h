#pragma once
#include "hitable.h"
class FlipNormals : public Hitable {
public:
	FlipNormals() {}
	FlipNormals(Hitable *hitable_ptr) :hitable_ptr_(hitable_ptr) {}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
		if (hitable_ptr_->Hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const {
		return hitable_ptr_->GetBoundingBox(t0, t1, box);
	}
	Hitable * hitable_ptr_;
};