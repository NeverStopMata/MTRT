#pragma once
#include "material.h"
class Dielectric : public Material {
public:
	Dielectric(float ri) :ref_idx_(ri) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const {
		srec.is_specular = true;
		srec.pdf_ptr = nullptr;
		Vec3 outward_normal;
		Vec3 reflected = Reflect(r_in.GetDirection(), hrec.normal);
		float ni_over_nt;
		srec.attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflect_prob;
		float cosine;
		if (Dot(r_in.GetDirection(), hrec.normal) > 0) {
			outward_normal = -hrec.normal;
			ni_over_nt = ref_idx_;
			cosine = ref_idx_ * Dot(r_in.GetDirection(), hrec.normal) / r_in.GetDirection().GetLength();
		}
		else {
			outward_normal = hrec.normal;
			ni_over_nt = 1.0f / ref_idx_;
			cosine = -Dot(r_in.GetDirection(), hrec.normal) / r_in.GetDirection().GetLength();
		}
		if (Refract(r_in.GetDirection(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = Schlick(cosine, ref_idx_);
		}
		else {
			reflect_prob = 1.0;
		}
		if ((double)rand() / RAND_MAX < reflect_prob) {
			srec.specular_ray = Ray(hrec.pos, reflected, r_in.GetTime());
		}
		else {
			srec.specular_ray = Ray(hrec.pos, refracted, r_in.GetTime());
		}
		return true;
	}
	float ref_idx_;
};