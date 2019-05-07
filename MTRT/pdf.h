#pragma once
#include "vec3.h"
class PDF {
public:
	virtual float GetValue(const Vec3& dir) const = 0;
	virtual Vec3 Generate() const = 0;
};