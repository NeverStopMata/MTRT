// MTRT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include<fstream>
#include <iostream>
#include <vector>
#include <random>

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3((double)rand() / (RAND_MAX)- 0.5, (double)rand() / (RAND_MAX)- 0.5, (double)rand() / (RAND_MAX)- 0.5);
	} while (p.squared_length() >= 1.0);
	return p;
}
vec3 color(const ray& r,hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		vec3 target = rec.pos + rec.normal + random_in_unit_sphere();
		return 0.5 * color(ray(rec.pos,target - rec.pos), world);
	}
	else {
		vec3 normalized_drct = normalize(r.direction());
		float t = 0.5f * (normalized_drct.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}
int main()
{
	std::ofstream OutFile("Test.ppm");
	OutFile.clear();
	int nx = 2000;
	int ny = 1000;
	int ns = 4;
	hitable *TempList[2];
	TempList[0] = new sphere(vec3(0, 0, -1), 0.5);
	TempList[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(TempList, 2);
	camera cam;
	std::vector<char> ResultBuffer(nx * ny * 3);
	int CharCnt = 0;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + 0.25f * (1 + 2 * float(s % 2))) / float(nx);
				float v = float(j + 0.25f * (1 + 2 * float(s / 2))) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world);
			}
			col /= float(ns);

			ResultBuffer[CharCnt++] = char(255.99*sqrt(col[0]));
			ResultBuffer[CharCnt++] = char(255.99*sqrt(col[1]));
			ResultBuffer[CharCnt++] = char(255.99*sqrt(col[2]));
		}
	}
	stbi_write_png("Test.png", nx, ny, 3, ResultBuffer.data(), 0);
	OutFile.close();
}