#pragma once
#include "vec3.h"
class Texture {
public:
	virtual Vec3 Sample(float u, float v, const Vec3& p) const = 0;
};