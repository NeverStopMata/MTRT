#pragma once
#include "pdf.h"
#include "material.h"
class UniformPDF :public PDF {
public:
	UniformPDF() {}
	virtual float GetValue(const Vec3& dir) const {
		return 0.25 / M_PI;
	}
	virtual Vec3 Generate() const {
		float r1 = GetRandom01();
		float r2 = GetRandom01();
		float phi = 2 * M_PI * r1;
		float z = 1 - 2 * r2;
		float x = cos(phi) * sqrt(1 - z * z);
		float y = sin(phi) * sqrt(1 - z * z);
		return Vec3(x, y, z);
	}
};