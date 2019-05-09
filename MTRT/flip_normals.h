#pragma once
#include "hitable.h"
class FlipNormals : public Hitable {
public:
	FlipNormals() {}
	~FlipNormals() {
		delete hitable_ptr_;
	}
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

	virtual float GetPDFValue(const Vec3& origin, const Vec3& dir) const {
		return hitable_ptr_->GetPDFValue(origin, dir);
	}
	virtual Vec3 GetRandomDirToSelf(const Vec3& origin) const {
		return hitable_ptr_->GetRandomDirToSelf(origin);
	}

	Hitable * hitable_ptr_;
};