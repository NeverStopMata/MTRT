#pragma once
#include "pdf.h"
#include "onb.h"
class CosinePDF :public PDF {
public:
	CosinePDF(const Vec3& w) { uvw_.BuildFormW(w); }
	virtual float GetValue(const Vec3& dir) const {
		float cosine = Dot(Normalize(dir), uvw_.w());
		if (cosine > 0)
			return cosine / M_PI;
		else
			return 0;
	}
	virtual Vec3 Generate() const {
		return uvw_.TransformToWorld(GetRandomCosineDirection());
	}
	ONB uvw_;
};