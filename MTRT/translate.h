#pragma once
#include "hitable.h"
class Translate : public Hitable {
public:
	Translate(Hitable * hitable_ptr, const Vec3& displacement) :hitable_ptr_(hitable_ptr),offset_(displacement){}
	~Translate() {
		delete hitable_ptr_;
	}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const;
	virtual float GetPDFValue(const Vec3& origin, const Vec3& dir) const;
	virtual Vec3 GetRandomDirToSelf(const Vec3& origin) const;
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

float Translate::GetPDFValue(const Vec3& origin, const Vec3& dir) const {
	return hitable_ptr_->GetPDFValue(origin - offset_, dir);
}
Vec3 Translate::GetRandomDirToSelf(const Vec3& origin) const{
	return hitable_ptr_->GetRandomDirToSelf(origin - offset_);
}