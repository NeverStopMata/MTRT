// MTRT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <random>


Vec3 GetColorByRay(const Ray& r,Hitable *world,int depth) {
	HitRecord rec;
	if (world->Hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->Scatter(r, rec, attenuation, scattered)){
			return attenuation * GetColorByRay(scattered, world,depth + 1);
		}
		else{
			return Vec3(0, 0, 0);
		}
		
	}
	else {
		Vec3 normalized_drct = Normalize(r.GetDirection());
		float t = 0.5f * (normalized_drct.y() + 1.0f);
		return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
	}
}
int main()
{
	int nx = 2000;
	int ny = 1000;
	int ns = 100;
	Hitable* hitable_list[5];
	hitable_list[0] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
	hitable_list[1] = new Sphere(Vec3(0,      0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)));
	hitable_list[2] = new Sphere(Vec3(1,      0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2),0.1));
	hitable_list[3] = new Sphere(Vec3(-1,     0, -1), 0.5, new Dielectric(1.5));
	hitable_list[4] = new MovingSphere(Vec3(0.5, 0, 0), Vec3(0.5, 0.3, 0), 0.0,1.0,0.2, new Lambertian(Vec3(0,1,1)));
	Hitable *world = new Hitable_list(hitable_list, 5);

	Vec3 look_from(-4, 1.5, 2);
	Vec3 look_at(0, 0, -1);
	float dist_to_focus = (look_from - look_at).GetLength();
	float aperture = 0.0;
	Camera cam(look_from, look_at,Vec3(0,1,0),20,float(nx)/float(ny), aperture, dist_to_focus,0.0,1.0);
	std::vector<char> result_buffer(nx * ny * 3);
	int char_cnt = 0;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			Vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + (double)rand() / RAND_MAX) / float(nx);
				float v = float(j + (double)rand() / RAND_MAX) / float(ny);
				Ray r = cam.GetRay(u, v);
				col += GetColorByRay(r, world, 0);
			}
			col /= float(ns);
			result_buffer[char_cnt++] = char(255.99*sqrt(col[0]));
			result_buffer[char_cnt++] = char(255.99*sqrt(col[1]));
			result_buffer[char_cnt++] = char(255.99*sqrt(col[2]));
		}
	}
	stbi_write_png("test_res.png", nx, ny, 3, result_buffer.data(), 0);
}