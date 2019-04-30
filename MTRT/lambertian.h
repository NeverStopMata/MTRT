#pragma once
#include "material.h"
#include "texture.h"
class Lambertian : public Material {
public:
	Lambertian(Texture const * a) : albedo_(a) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenunation, Ray& scattered) const {
		Vec3 target = rec.pos + rec.normal + GetRndmVecInUnitSphere();
		scattered = Ray(rec.pos, target - rec.pos,r_in.GetTime());
		attenunation = albedo_->Sample(rec.u,rec.v,rec.pos);
		return true;
	}
	Texture const * albedo_;
};
