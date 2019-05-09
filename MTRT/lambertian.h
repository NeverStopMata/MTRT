#pragma once
#include "material.h"
#include "texture.h"
#include "onb.h"
#include "cosine_pdf.h"
class Lambertian : public Material {
public:
	Lambertian(Texture const * a) : albedo_(a) {}
	~Lambertian() {
		delete albedo_;
	}
	virtual float GetScatteringPDF(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
		float cosine = Dot(rec.normal, Normalize(scattered.GetDirection()));
		if (cosine < 0) cosine = 0;
		return cosine / M_PI;
	}
	virtual bool Scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec)  const {
		srec.is_specular = false;
		srec.attenuation = albedo_->Sample(hrec.u, hrec.v, hrec.pos);
		srec.pdf_ptr = std::make_shared<CosinePDF>(hrec.normal);
		return true;
	}
	Texture const * albedo_;
};
