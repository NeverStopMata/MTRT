#pragma once
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hitable_list.h"
#include "bvh_node.h"
#include "scene_creator.h"
#include "image_texture.h"
#include "sky_box.h"
#include "sky_box_mat.h"
#include "camera.h"
#include "cosine_pdf.h"
#include "hitable_pdf.h"
#include "mixture_pdf.h"
#include "perlin.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>
#include <string>

Vec3 GetColorByRay(const Ray& r, std::shared_ptr<Hitable> world, std::shared_ptr<Hitable> light_shape, int depth) {

	HitRecord hrec;
	if (world->Hit(r, FLOAT_EPSILON, FLT_MAX, hrec)) {
		ScatterRecord srec;
		Vec3 emitted = hrec.mat_ptr->Emitted(r, hrec);
		if (depth < 50 && hrec.mat_ptr->Scatter(r, hrec, srec)) {

			if (srec.is_specular) {
				return srec.attenuation * GetColorByRay(srec.specular_ray, world, light_shape, depth + 1);
			}
			HitablePDF pdf_to_important(light_shape, hrec.pos);
			MixturePDF pdf_mix(std::make_shared<HitablePDF>(pdf_to_important), srec.pdf_ptr);
			Ray scattered = Ray(hrec.pos, pdf_mix.Generate(), r.GetTime());
			float pdf_val = pdf_mix.GetValue(scattered.GetDirection());
			if (pdf_val < FLOAT_EPSILON)
				return emitted;
			return emitted + srec.attenuation * hrec.mat_ptr->GetScatteringPDF(r, hrec, scattered)
				* GetColorByRay(scattered, world, light_shape, depth + 1) / pdf_val;

		}
		else {
			return emitted;
		}
	}
	else {
		return Vec3(0, 0, 0);
	}

}

class RayTracer {
public:
	static void DrawPicture();
};

void RayTracer::DrawPicture() {
	bool preview = 0;
	int nx = preview ? 20 : 800;
	int ny = preview ? 20 : 800;
	int ns = preview ? 10 : 2000;
	int start_time = time(0);
	std::vector<std::shared_ptr<Hitable>> obj_list, important_list;

	SceneCreator::CreateCornellBox(obj_list, important_list);
	auto important_hitables = std::make_shared<Hitable_list>(important_list);
	auto obj_bvh_tree = std::make_shared<BVHNode>(obj_list.begin(), obj_list.end(), 0, 1);
	int sky_tex_nx, sky_tex_ny, nn;
	unsigned char *tex_data = stbi_load("resources/textures/snow.jpg", &sky_tex_nx, &sky_tex_ny, &nn, 0);
	std::shared_ptr<Material> sky_mat(new SkyBoxMat(new ImageTexture(tex_data, sky_tex_nx, sky_tex_ny)));
	auto sky_box = std::make_shared<SkyBox>(Vec3(0, 0, 0), 500, sky_mat);
	std::vector<std::shared_ptr<Hitable>> hitable_array = { obj_bvh_tree };
	auto world = std::make_shared<Hitable_list>(hitable_array);

	Vec3 look_from(278, 278, -800);
	Vec3 look_at(278, 278, 0);
	float dist_to_focus = 10;
	float aperture = 0.0f;
	float fov = 40.0f;
	Camera cam(look_from, look_at, Vec3(0, 1, 0), fov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
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
				r.direction_.normalize();
				col += DeNAN(GetColorByRay(r, world, important_hitables, 0));
			}
			col /= float(ns);
			result_buffer[char_cnt++] = char(255.99* std::min(sqrt(col[0]), 1.0f));
			result_buffer[char_cnt++] = char(255.99* std::min(sqrt(col[1]), 1.0f));
			result_buffer[char_cnt++] = char(255.99* std::min(sqrt(col[2]), 1.0f));
		}
	}
	std::stringstream file_name;
	std::cout << time(0) - start_time << std::endl;
	file_name << "result/temp/" << "test_res.png";
	stbi_write_png(file_name.str().c_str(), nx, ny, 3, result_buffer.data(), 0);
	Perlin::ReleaseBuffer();
}
