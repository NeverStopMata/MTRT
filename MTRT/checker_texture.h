#pragma once
#include "texture.h"
class CheckerTexture :public Texture {
public:
	CheckerTexture() {}
	CheckerTexture(Texture const * t0, Texture const * t1) : odd_tex_(t0), even_tex_(t1) {}
	virtual Vec3 Sample(float u, float v, const Vec3& p) const {
		float sin = std::sin(10 * p.x()) * std::sin(10 * p.y()) * std::sin(10 * p.z());
		if (sin < 0)
			return odd_tex_->Sample(u, v, p);
		else
			return even_tex_->Sample(u, v, p);
	}
	Texture const * odd_tex_;
	Texture const * even_tex_;
};