#pragma once
#include "material.h"
class Metal : public Material {
public:
	Metal(const Vec3& a, float f) : albedo_(a) { if (f < 1) fuzz_ = f; else fuzz_ = 1; }
	virtual bool Scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec)const {
		if (Dot(r_in.GetDirection(), hrec.normal) > 0)
			return false;
		Vec3 reflected = Reflect(Normalize(r_in.GetDirection()), hrec.normal);
		srec.specular_ray = Ray(hrec.pos, reflected + fuzz_ * GetRndmVecInUnitSphere(), r_in.GetTime());
		srec.attenuation = albedo_;
		srec.is_specular = true;
		srec.pdf_ptr = nullptr;
		return true;
	}
	Vec3 albedo_;
	float fuzz_;
};