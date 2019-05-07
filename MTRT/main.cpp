// MTRT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hitable_list.h"
#include "bvh_node.h"
#include "scene_creator.h"
#include "sky_box.h"
#include "sky_box_mat.h"
#include "camera.h"
#include "cosine_pdf.h"
#include "hitable_pdf.h"
#include "mixture_pdf.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>
#include <string>



Vec3 GetColorByRay(const Ray& r,Hitable *world,ImageTexture* skybox_tex,int depth) {
	HitRecord rec;
	if (world->Hit(r, FLOAT_EPSILON, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 albedo;
		Vec3 emitted = rec.mat_ptr->Emitted(r,rec);
		float pdf_val;
		if (depth < 50 && rec.mat_ptr->Scatter(r, rec, albedo, scattered, pdf_val)){
			 //CosinePDF pdf(rec.normal);
			Hitable* light_shape = new XZRect(213, 343, 227, 332, 554, 0);
			HitablePDF pdf0(light_shape, rec.pos);
			CosinePDF pdf1(rec.normal);
			MixturePDF pdf_mix(&pdf0, &pdf1);
			scattered = Ray(rec.pos, pdf_mix.Generate(), r.GetTime());
			pdf_val = pdf_mix.GetValue(scattered.GetDirection());
			if (pdf_val < FLOAT_EPSILON)
				return emitted;
			return emitted + albedo * rec.mat_ptr->GetScatteringPDF(r,rec,scattered)
						* GetColorByRay(scattered, world, skybox_tex,depth + 1) / pdf_val;
		}
		else{
			return emitted;
		}	
	}
	else {
		return Vec3(0, 0, 0);
	}
}
int main()
{
	bool preview = 1;
	int nx = preview ?200:1000;
	int ny = preview ?200:1000;
	int ns = preview ?1000  :100;
	int start_time = time(0);
	std::vector<Hitable*> obj_list = SceneCreator::CreateCornellBox();
	Hitable *obj_bvh_tree = new BVHNode(obj_list.begin(), obj_list.end(),0,1);
	int sky_tex_nx, sky_tex_ny, nn;
	unsigned char *tex_data = stbi_load("resources/textures/snow.jpg", &sky_tex_nx, &sky_tex_ny, &nn, 0);
	auto skybox_tex = new ImageTexture(tex_data, sky_tex_nx, sky_tex_ny);
	SkyBox* sky_box = new SkyBox(Vec3(0, 0, 0), 500, new SkyBoxMat(skybox_tex));
	std::vector<Hitable*> hitable_array = { obj_bvh_tree };
	Hitable_list* world = new Hitable_list(hitable_array);

	Vec3 look_from(278, 278, -800);
	Vec3 look_at(278, 278, 0);
	float dist_to_focus = 10;
	float aperture = 0.0f;
	float fov = 40.0f;
	Camera cam(look_from, look_at,Vec3(0,1,0), fov,float(nx)/float(ny), aperture, dist_to_focus,0.0,1.0);
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
				col += DeNAN(GetColorByRay(r, world, skybox_tex,0));
			}
			col /= float(ns);
			result_buffer[char_cnt++] = char(255.99* std::min(sqrt(col[0]), 1.0f)); 
			result_buffer[char_cnt++] = char(255.99* std::min(sqrt(col[1]), 1.0f));
			result_buffer[char_cnt++] = char(255.99* std::min(sqrt(col[2]), 1.0f));
		}
	}
	std::stringstream file_name;
	std::cout << time(0) - start_time << std::endl;
	file_name << "result/temp/"<<"test_res.png";
	stbi_write_png(file_name.str().c_str(), nx, ny, 3, result_buffer.data(), 0);
}