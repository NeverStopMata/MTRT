#pragma once
#include "texture.h"
#include "perlin.h"
class NoiseTexture : public Texture {
public:
	NoiseTexture() {}
	NoiseTexture(float scale) : scale_(scale) {}
	virtual Vec3 Sample(float u, float v, const Vec3& p) const {
		//return Vec3(1, 1, 1) * noise_.GetNoiseValueByPos(scale_ * p);
		return Vec3(1, 1, 1) * 0.5 * (1 + sin(scale_*p.z() + 10 * noise_.Turb(p)));
	}
	Perlin noise_;
	float scale_;
};