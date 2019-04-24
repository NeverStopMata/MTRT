#pragma once
#include "material.h"
class Lambertian : public Material {
public:
	Lambertian(const Vec3& a) : albedo_(a) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenunation, Ray& scattered) const {
		Vec3 target = rec.pos + rec.normal + GetRndmVecInUnitSphere();
		scattered = Ray(rec.pos, target - rec.pos,r_in.GetTime());
		attenunation = albedo_;
		return true;
	}
	Vec3 albedo_;
};
