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
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>
#include <string>



Vec3 GetColorByRay(const Ray& r,Hitable *world,ImageTexture* skybox_tex,int depth) {
	HitRecord rec;
	if (world->Hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attenuation;
		Vec3 emitted = rec.mat_ptr->Emitted(rec.u, rec.v, rec.pos);
		if (depth < 50 && rec.mat_ptr->Scatter(r, rec, attenuation, scattered)){
			scattered.direction_.normalize();
			return emitted + attenuation * GetColorByRay(scattered, world, skybox_tex,depth + 1);
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
	bool preview = 0;
	int nx = preview ?200:1920;
	int ny = preview ?100:1080;
	int ns = preview ?10  :100;
	int start_time = time(0);
	std::vector<Hitable*> obj_list = SceneCreator::CreateLightScene();
	Hitable *obj_bvh_tree = new BVHNode(obj_list.begin(), obj_list.end(),0,1);
	int sky_tex_nx, sky_tex_ny, nn;
	unsigned char *tex_data = stbi_load("resources/textures/snow.jpg", &sky_tex_nx, &sky_tex_ny, &nn, 0);
	auto skybox_tex = new ImageTexture(tex_data, sky_tex_nx, sky_tex_ny);
	SkyBox* sky_box = new SkyBox(Vec3(0, 0, 0), 500, new SkyBoxMat(skybox_tex));
	std::vector<Hitable*> hitable_array = { obj_bvh_tree ,sky_box };
	Hitable_list* world = new Hitable_list(hitable_array);

	
	

	Vec3 look_from(8, 4, 8);
	Vec3 look_at(0, 0, 0);
	float dist_to_focus = (look_from - look_at).GetLength();
	float aperture = 0.0f;
	Camera cam(look_from, look_at,Vec3(0,1,0),35,float(nx)/float(ny), aperture, dist_to_focus,0.0,1.0);
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
				col += GetColorByRay(r, world, skybox_tex,0);
			}
			col /= float(ns);
			result_buffer[char_cnt++] = char(255.99*sqrt(col[0]));
			result_buffer[char_cnt++] = char(255.99*sqrt(col[1]));
			result_buffer[char_cnt++] = char(255.99*sqrt(col[2]));
		}
	}
	std::stringstream file_name;
	std::cout << time(0) - start_time << std::endl;
	file_name << "result/temp/"<<"test_res.png";
	stbi_write_png(file_name.str().c_str(), nx, ny, 3, result_buffer.data(), 0);
}