#pragma once
#include "ray.h"
class camera {
public:
	camera() {
		LowerLeftCorner = vec3(-2.0, -1.0, -1.0);
		Horizontal = vec3(4.0, 0.0, 0.0);
		Vertical = vec3(0.0, 2.0, 0.0);
		Origin = vec3(0.0, 0.0, 0.0);
	}
	ray get_ray(float u, float v) {
		return ray(Origin, LowerLeftCorner + u * Horizontal + v * Vertical - Origin);
	}
	vec3 Origin;
	vec3 LowerLeftCorner;
	vec3 Horizontal;
	vec3 Vertical;
};