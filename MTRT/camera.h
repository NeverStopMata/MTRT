#pragma once
#include "ray.h"
constexpr auto M_PI = 3.14159265358979323846f;
class Camera {
public:
	Camera(Vec3 look_from,Vec3 look_at, Vec3 vec_up, float vertical_fov, float aspect, float aperture, float focus_dist, float t0, float t1) : origin_(look_from),time0_(t0),time1_(t1){
		lens_radius_ = aperture / 2.0;
		float theta = vertical_fov * M_PI / 180.0;
		float half_height = tan(theta / 2.0);
		float half_width = aspect * half_height;
		w_ = Normalize(look_from - look_at);
		u_ = Normalize(Cross(vec_up, w_));
		v_ = Cross(w_, u_);
		lower_left_corner_ = origin_ - half_width * focus_dist * u_ - half_height * focus_dist * v_ - focus_dist * w_;
		horizontal_ = 2 * half_width  * focus_dist * u_;
		vertical_   = 2 * half_height * focus_dist * v_;
	}
	Ray GetRay(float s, float t) {
		float time = time0_ + ((double)rand() / RAND_MAX) * (time1_ - time0_);
		Vec3 vec_random = lens_radius_ * GetRndmVecInUnitSphere();
		Vec3 offset = u_ * vec_random.x() + v_ * vec_random.y();
		Vec3 origin_after_offset = origin_ + offset;
		return Ray(origin_after_offset, lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_after_offset, time);
	}
	Vec3 origin_;
	Vec3 lower_left_corner_;
	Vec3 horizontal_;
	Vec3 vertical_;
	Vec3 u_, v_, w_;
	float time0_, time1_;
	float lens_radius_;
};