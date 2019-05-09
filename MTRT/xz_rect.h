#pragma once
#include "hitable.h"
#include "ray.h"
class XZRect : public Hitable {
public:
	XZRect() {}
	~XZRect() {
	}
	XZRect(float x0, float x1, float z0, float z1, float k, std::shared_ptr<Material> mat_ptr) :x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), mat_ptr_(mat_ptr) {};
	virtual bool Hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb& box) const {
		box = Aabb(Vec3(x0_, k_ - FLOAT_EPSILON, z0_), Vec3(x1_, k_ + FLOAT_EPSILON, z1_));
		return true;
	}
	virtual float GetPDFValue(const Vec3& origin, const Vec3& dir) const { 
		HitRecord rec;
		if (this->Hit(Ray(origin, dir), FLOAT_EPSILON, FLT_MAX, rec)) {
			float area = (x1_ - x0_)*(z1_ - z0_);
			float dist_squared = rec.t * rec.t * dir.GetSquaredLength();
			float cosine = fabs(Dot(dir, rec.normal) / dir.GetLength());
			if (cosine < FLOAT_EPSILON)
				return 0;
			return dist_squared / (cosine * area);
		}
		else
			return 0;
	}
	virtual Vec3 GetRandomDirToSelf(const Vec3& origin) const{
		Vec3 random_pos = Vec3(x0_ + GetRandom01()*(x1_ - x0_), k_, z0_ + GetRandom01()*(z1_ - z0_));
		return random_pos - origin;
	}

	std::shared_ptr<Material> mat_ptr_;
	float x0_, x1_, z0_, z1_, k_;
};

bool XZRect::Hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
	float t = (k_ - r.GetOrigin().y()) / r.GetDirection().y();
	if (t < t0 || t > t1)
		return false;
	float x = r.GetOrigin().x() + t * r.GetDirection().x();
	if (x < x0_ || x > x1_)
		return false;
	float z = r.GetOrigin().z() + t * r.GetDirection().z();
	if (z < z0_ || z > z1_)
		return false;
	rec.u = (x - x0_) / (x1_ - x0_);
	rec.v = (z - z0_) / (z1_ - z0_);
	rec.t = t;
	rec.mat_ptr = mat_ptr_;
	rec.pos = r.GetPointAtParameter(t);
	rec.normal = Vec3(0, 1, 0);
	return true;
}