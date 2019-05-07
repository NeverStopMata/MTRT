#pragma once
#include "material.h"
#include "texture.h"
#include "ray.h"
class Isotropic : public Material {
public:
	Isotropic(Texture* tex) :albedo_(tex) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered)const {
		scattered = Ray(rec.pos, GetRndmVecInUnitSphere());
		attenuation = albedo_->Sample(rec.u, rec.v, rec.pos);
		return true;
	}
	Texture * albedo_;
};