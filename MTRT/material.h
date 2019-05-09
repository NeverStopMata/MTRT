#pragma once
#include "hitable.h"
#include "pdf.h"
struct ScatterRecord
{
	Ray specular_ray;
	bool is_specular;
	Vec3 attenuation;
	std::shared_ptr<PDF> pdf_ptr;
};
class Material {
public:
	virtual ~Material() {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& hrec, ScatterRecord& srec) const {
		return false;
	}
	virtual float GetScatteringPDF(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
		return 0.0f;
	}
	virtual Vec3  Emitted(const Ray& r_in, const HitRecord& rec) const {
		return Vec3(0, 0, 0);
	}
};

Vec3 GetRndmVecInUnitSphere() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(GetRandom01() - 0.5, GetRandom01() - 0.5, GetRandom01() - 0.5);
	} while (p.GetSquaredLength() >= 1.0);
	return p;
}
Vec3 Reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * Dot(v, n)*n;
}
bool Refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
	Vec3 unit_v = Normalize(v);
	float v_dot_n = Dot(unit_v, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - v_dot_n * v_dot_n);//cos(transmit degree)^2
	if (discriminant > 0) {
		refracted = ni_over_nt * (unit_v - n * v_dot_n) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}
float Schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}