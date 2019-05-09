#pragma once
#include "material.h"
#include "texture.h"
#include "ray.h"
#include "uniform_pdf.h"
class Isotropic : public Material {
public:
	Isotropic(Texture* tex) :albedo_(tex) {}
	~Isotropic(){
		delete albedo_;
	}
	virtual bool Scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const {
		srec.is_specular = false;
		srec.attenuation = albedo_->Sample(hrec.u, hrec.v, hrec.pos);
		srec.pdf_ptr = std::make_shared<UniformPDF>();
		return true;
	}
	virtual float GetScatteringPDF(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
		return 0.25 / M_PI;
	}
	Texture * albedo_;
};