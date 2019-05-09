#pragma once
#include "hitable.h"
#include "onb.h"
class Sphere : public Hitable {
public:
	Sphere() {}
	~Sphere() {
	}
	Sphere(Vec3 cen, float r,std::shared_ptr<Material> mat) :center_(cen), radius_(r), mat_ptr_(mat){};
	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool GetBoundingBox(float t0, float t1, Aabb & box) const;
	virtual float GetPDFValue(const Vec3& origin, const Vec3& dir) const;
	virtual Vec3 GetRandomDirToSelf(const Vec3& origin) const;
	Vec3 center_;
	float radius_;
	std::shared_ptr<Material> mat_ptr_;
};

bool Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	Vec3 vec_centr_orign = r.GetOrigin() - center_;
	float a = Dot(r.GetDirection(), r.GetDirection());
	float b = Dot(vec_centr_orign, r.GetDirection());
	float c = Dot(vec_centr_orign, vec_centr_orign) - radius_ * radius_;
	float discriminant = b * b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.pos = r.GetPointAtParameter(rec.t);
			rec.normal = (rec.pos - center_) / radius_;
			rec.mat_ptr = mat_ptr_;
			GetSphereUV((rec.pos - center_) / radius_, rec.u, rec.v);
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
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

bool Sphere::GetBoundingBox(float t0, float t1, Aabb& box) const
{
	box = Aabb(center_ - Vec3(radius_, radius_, radius_), center_ + Vec3(radius_, radius_, radius_));
	return true;
}

float Sphere::GetPDFValue(const Vec3& origin, const Vec3& dir) const {
	if ((origin - center_).GetLength() < radius_) // if the origin is inner the sphere, uniform distribution is used.
		return 0.25 / M_PI;
	HitRecord hrec;
	if (this->Hit(Ray(origin, dir), FLOAT_EPSILON, FLT_MAX, hrec)) {
		float cos_theta_max = sqrt(1 - radius_ * radius_ / (center_ - origin).GetSquaredLength());
		float solid_angle = 2 * M_PI * (1 - cos_theta_max);
		return 1 / solid_angle;
	}
	else
		return 0;
}
Vec3 Sphere::GetRandomDirToSelf(const Vec3& origin) const {
	if ((origin - center_).GetLength() < radius_) // if the origin is inner the sphere, uniform distribution is used.
		return GetRandomDirOnSphere();
	Vec3 direction = center_ - origin;
	float dist_squared = direction.GetSquaredLength();
	ONB uvw;
	uvw.BuildFormW(direction);
	return uvw.TransformToWorld(GetRandomDirToSphere(radius_, dist_squared));
}