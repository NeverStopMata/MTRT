#pragma once
#include "texture.h"
class ConstTexture : public Texture {
public:
	ConstTexture() {}
	ConstTexture(Vec3 c) : color_(c) {}
	virtual Vec3 Sample(float u, float v, const Vec3& p)const {
		return color_;
	}
	Vec3 color_;
};