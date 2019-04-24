#pragma once
#include "material.h"
class Dielectric : public Material {
public:
	Dielectric(float ri) :ref_idx_(ri) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 outward_normal;
		Vec3 reflected = Reflect(r_in.GetDirection(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflect_prob;
		float cosine;
		if (Dot(r_in.GetDirection(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx_;
			cosine = ref_idx_ * Dot(r_in.GetDirection(), rec.normal) / r_in.GetDirection().GetLength();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx_;
			cosine = -Dot(r_in.GetDirection(), rec.normal) / r_in.GetDirection().GetLength();
		}
		if (Refract(r_in.GetDirection(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = Schlick(cosine, ref_idx_);
		}
		else {
			reflect_prob = 1.0;
		}
		if ((double)rand() / RAND_MAX < reflect_prob) {
			scattered = Ray(rec.pos, reflected, r_in.GetTime());
		}
		else {
			scattered = Ray(rec.pos, refracted, r_in.GetTime());
		}
		return true;
	}
	float ref_idx_;
};