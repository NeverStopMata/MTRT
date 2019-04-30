#pragma once
#include "material.h"
#include "texture.h"
class DiffuseLight : public Material {
public:
	DiffuseLight() {}
	DiffuseLight(Texture* emit_texture,float intensity): emit_texture_( emit_texture),inensity_(intensity){}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenunation, Ray& scattered) const {
		return false;
	}
	virtual Vec3 Emitted(float u, float v, const Vec3& p) const {
		return emit_texture_->Sample(u,v,p) * inensity_;
	}
	Texture* emit_texture_;
	float inensity_;
};