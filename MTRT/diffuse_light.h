#pragma once
#include "material.h"
#include "texture.h"
class DiffuseLight : public Material {
public:
	DiffuseLight() {}
	~DiffuseLight() {
		delete emit_texture_;
	}
	DiffuseLight(Texture* emit_texture,float intensity): emit_texture_( emit_texture),inensity_(intensity){}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenunation, Ray& scattered) const {
		return false;
	}
	virtual Vec3 Emitted(const Ray& r_in, const HitRecord& rec) const {
		if (Dot(rec.normal, r_in.GetDirection()) < -FLOAT_EPSILON)
			return emit_texture_->Sample(rec.u, rec.v, rec.pos) * inensity_;
		else
			return Vec3(0, 0, 0);
	}
	Texture* emit_texture_;
	float inensity_;
};