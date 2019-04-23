#pragma once
#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r) :center(cen), radius(r) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 VecCntr2Orgn = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(VecCntr2Orgn, r.direction());
	float c = dot(VecCntr2Orgn, VecCntr2Orgn) - radius * radius;
	float Discriminant = b * b - a*c;
	if (Discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.pos = r.point_at_parameter(rec.t);
			rec.normal = (rec.pos - center) / radius;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.pos = r.point_at_parameter(rec.t);
			rec.normal = (rec.pos - center) / radius;
			return true;
		}
	}
	return false;
}