#pragma once
#include "material.h"
class Metal : public Material {
public:
	Metal(const Vec3& a, float f) : albedo_(a) { if (f < 1) fuzz_ = f; else fuzz_ = 1; }
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenunation, Ray& scattered)const {
		Vec3 reflected = Reflect(Normalize(r_in.GetDirection()), rec.normal);
		scattered = Ray(rec.pos, reflected + fuzz_ * GetRndmVecInUnitSphere(), r_in.GetTime());
		attenunation = albedo_;
		return (Dot(scattered.GetDirection(), rec.normal) > 0);
	}
	Vec3 albedo_;
	float fuzz_;
};