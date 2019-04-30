#pragma once
#include "vec3.h"
inline float TrilinearInterp(Vec3 c[2][2][2], float u, float v, float w) {
	float non_linear_u = u * u * (3 - 2 * u);
	float non_linear_v = v * v * (3 - 2 * v);
	float non_linear_w = w * w * (3 - 2 * w);
	float accum = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++) {
				Vec3 weight_v(u - i, v - j, w - k);
				accum += (i * non_linear_u + (1 - i) * (1 - non_linear_u)) *
						 (j * non_linear_v + (1 - j) * (1 - non_linear_v)) *
						 (k * non_linear_w + (1 - k) * (1 - non_linear_w)) * 
						 Dot(c[i][j][k], weight_v);
			}
	return accum;
}

class Perlin {
public:
	float GetNoiseValueByPos(const Vec3& p)const {
		float u = p.x() - floorf(p.x());
		float v = p.y() - floorf(p.y());
		float w = p.z() - floorf(p.z());
		//u = u * u * (3 - 2 * u);
		//v = v * v * (3 - 2 * v);
		//w = w * w * (3 - 2 * w);
		int i = floorf(p.x());
		int j = floorf(p.y());
		int k = floorf(p.z());
		Vec3 c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = random_vec_arry_[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
		return TrilinearInterp(c,u,v,w);
	}
	float Turb(const Vec3& p, int depth = 7)const {
		float accum = 0;
		Vec3 temp_p = p;
		float weight = 1.0;
		for (int i = 0; i < depth; i++) {
			accum += weight * GetNoiseValueByPos(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}
		return fabs(accum);
	}
	static Vec3* random_vec_arry_;
	static int* perm_x;
	static int* perm_y;
	static int* perm_z;
};

static Vec3* GenerateRandomVecArray() {
	Vec3* p = new Vec3[256];
	for (int i = 0; i < 256; ++i) {
		p[i] = Normalize(Vec3(-1 + 2*GetRandom01(), -1 + 2 * GetRandom01(), -1 + 2 * GetRandom01()));
	}
	return p;
}

void Permute(int* p, int n) {
	for (int i = n - 1; i > 0; i--) {
		int target = int(GetRandom01()*(i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}
static int* GeneratePerlinPerm() {
	int* p = new int[256];
	for (int i = 0; i < 256; i++)
		p[i] = i;
	Permute(p, 256);
	return p;
}

Vec3* Perlin::random_vec_arry_ = GenerateRandomVecArray();
int* Perlin::perm_x = GeneratePerlinPerm();
int* Perlin::perm_y = GeneratePerlinPerm();
int* Perlin::perm_z = GeneratePerlinPerm();