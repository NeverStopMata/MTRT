#pragma once
#include "material.h"
#include "texture.h"
#include "onb.h"
class Lambertian : public Material {
public:
	Lambertian(Texture const * a) : albedo_(a) {}
	virtual float GetScatteringPDF(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
		float cosine = Dot(rec.normal, Normalize(scattered.GetDirection()));
		if (cosine < 0) cosine = 0;
		return cosine / M_PI;
	}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenunation, Ray& scattered, float& pdf) const {
		ONB uvw;
		uvw.BuildFormW(rec.normal);
		Vec3 direction = uvw.TransformToWorld(GetRandomCosineDirection());
		scattered = Ray(rec.pos, Normalize(direction),r_in.GetTime());
		attenunation = albedo_->Sample(rec.u,rec.v,rec.pos);
		pdf = Dot(rec.normal, scattered.GetDirection()) / M_PI;
		return true;
	}
	Texture const * albedo_;
};
