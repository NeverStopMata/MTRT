#pragma once
#include "hitable.h"
class MovingSphere : public Hitable {
public:
	MovingSphere() {}
	MovingSphere(Vec3 cen0,Vec3 cen1,float t0,float t1, float r, Material* mat):
		center0_(cen0), center1_(cen1), time0_(t0), time1_(t1), radius_(r), mat_ptr_(mat) {};
	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	Vec3 GetCenter(float time) const;

	Vec3 center0_,center1_;
	float time0_, time1_;
	float radius_;
	Material* mat_ptr_;
};

Vec3 MovingSphere::GetCenter(float time) const {
	return center0_ + ((time - time0_) / (time1_ - time0_)) * (center1_ - center0_);
}
bool MovingSphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	Vec3 VecCentrToOrgn = r.GetOrigin() - GetCenter(r.GetTime());
	float a = Dot(r.GetDirection(), r.GetDirection());
	float b = Dot(VecCentrToOrgn, r.GetDirection());
	float c = Dot(VecCentrToOrgn, VecCentrToOrgn) - radius_ * radius_;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.pos = r.GetPointAtParameter(rec.t);
			rec.normal = (rec.pos - GetCenter(r.GetTime())) / radius_;
			rec.mat_ptr = mat_ptr_;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.pos = r.GetPointAtParameter(rec.t);
			rec.normal = (rec.pos - GetCenter(r.GetTime())) / radius_;
			rec.mat_ptr = mat_ptr_;
			return true;
		}
	}
	return false;
}