#pragma once
#include "material.h"
#include "texture.h"
class SkyBoxMat : public Material {
public:
	SkyBoxMat(Texture const * a) : emit_(a) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenunation, Ray& scattered) const {
		return false;
	}
	virtual Vec3 Emitted(float u, float v, const Vec3& p) const {
		return emit_->Sample(u,v,p) * 0.2;
	}
	Texture const * emit_;
};

