#pragma once
#include <math.h>
#include <stdlib.h>
#include<iostream>
constexpr auto M_PI = 3.14159265358979323846f;
constexpr auto FLOAT_EPSILON = 0.000001f;


class Vec3 {
public:
	Vec3() {}
	Vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const Vec3& operator+()const { return *this; };
	inline Vec3 operator-()const { return Vec3(-e[0], -e[1], -e[2]); };
	inline float operator[](int i) const { 
		return e[i]; };
	inline float& operator[](int i) { return e[i]; };



	bool operator==(const Vec3& other) const {
		return e[0] == other.e[0] && e[1] == other.e[1] && e[2] == other.e[2];
	}

	inline Vec3& operator+=(const Vec3 &v2) {
		e[0] += v2[0];
		e[1] += v2[1];
		e[2] += v2[2];
		return *this;
	}
	inline Vec3& operator-=(const Vec3 &v2) {
		e[0] -= v2[0];
		e[1] -= v2[1];
		e[2] -= v2[2];
		return *this;
	}
	inline Vec3& operator*=(const Vec3 &v2) {
		e[0] *= v2[0];
		e[1] *= v2[1];
		e[2] *= v2[2];
		return *this;
	}
	inline Vec3& operator/=(const Vec3 &v2) {
		e[0] /= v2[0];
		e[1] /= v2[1];
		e[2] /= v2[2];
		return *this;
	}
	inline Vec3& operator*=(const float t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	inline Vec3& operator/=(const float t) {
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
		return *this;
	}
	inline float GetLength() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float squared_length() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void normalize() {
		float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
		e[0] *= k;
		e[1] *= k;
		e[2] *= k;
	}


	float e[3];
};

namespace std {
	template<> struct hash<Vec3> {
		size_t operator()(Vec3 const& vec) const {
			return ((hash<float>()(vec.e[0]) ^
				(hash<float>()(vec.e[1]) << 1)) >> 1) ^
				(hash<float>()(vec.e[2]));
		}
	};
}


inline std::istream& operator>>(std::istream &is, Vec3 &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3 &t) {
	os << t.e[0] << t.e[1] << t.e[2];
	return os;
}
inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}
inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}
inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}
inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}
inline Vec3 operator/(const Vec3 &v1, float t) {
	return Vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}
inline Vec3 operator*(const Vec3 &v1, float t) {
	return Vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}
inline Vec3 operator*(float t, const Vec3 &v1) {
	return Vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}
inline float Dot(const Vec3 &v1, const Vec3 &v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}
inline Vec3 Cross(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(
		v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vec3 Normalize(const Vec3 &v) {
	float k = 1.0f / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return Vec3(
		v[0] * k,
		v[1] * k,
		v[2] * k);
}

inline float GetRandom01() {
	return float((double)rand() / RAND_MAX);
}

void GetSphereUV(const Vec3& p, float& u, float& v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}