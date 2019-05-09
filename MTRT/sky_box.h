#pragma once
#include "hitable.h"

class SkyBox : public Hitable {
public:
	SkyBox() {}
	~SkyBox() {

	}
	SkyBox(Vec3 cen, float r, std::shared_ptr<Material> mat) :center_(cen), radius_(r), mat_ptr_(mat) {};
	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb & box) const;
	Vec3 center_;
	float radius_;
	std::shared_ptr<Material> mat_ptr_;
};

bool SkyBox::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	Vec3 vec_centr_orign = r.GetOrigin() - center_;
	float a = Dot(r.GetDirection(), r.GetDirection());
	float b = Dot(vec_centr_orign, r.GetDirection());
	float c = Dot(vec_centr_orign, vec_centr_orign) - radius_ * radius_;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.pos = r.GetPointAtParameter(rec.t);
			rec.normal = (rec.pos - center_) / radius_;
			rec.mat_ptr = mat_ptr_;
			GetSphereUV((rec.pos - center_) / radius_, rec.u, rec.v);
			return true;
		}
		temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.pos = r.GetPointAtParameter(rec.t);
			rec.normal = (rec.pos - center_) / radius_;
			rec.mat_ptr = mat_ptr_;
			return true;
		}
	}
	return false;
}

bool SkyBox::GetBoundingBox(float t0, float t1, Aabb& box) const
{
	return false;
}