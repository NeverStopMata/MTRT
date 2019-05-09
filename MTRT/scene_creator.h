#pragma once
#include <vector>
#include "hitable.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
#include "sphere.h"
#include "xy_rect.h"
#include "yz_rect.h"
#include "xz_rect.h"
#include "box.h"
#include "constant_medium.h"
#include "flip_normals.h"
#include "translate.h"
#include "rotate_y.h"
#include "triangle_mesh.h"
#include "const_texture.h"
#include "checker_texture.h"
#include "image_texture.h"
#include "noise_texture.h"
#include "diffuse_light.h"
extern _CrtMemState s1;
class SceneCreator {
public:
	

	static void CreateCornellBox(std::vector<std::shared_ptr<Hitable>>& obj_list, std::vector<std::shared_ptr<Hitable>>& important_list){
		obj_list.clear();
		important_list.clear();
		std::shared_ptr<Material> noise(new Lambertian(new NoiseTexture(4)));
		std::shared_ptr<Material> red(new Lambertian(new ConstTexture(Vec3(0.65, 0.05, 0.05))));
		std::shared_ptr<Material> white(new Lambertian(new ConstTexture(Vec3(0.73, 0.73, 0.73))));
		std::shared_ptr<Material> green(new Lambertian(new ConstTexture(Vec3(0.12, 0.45, 0.15))));
		std::shared_ptr<Material> aluminum(new Metal(Vec3(0.8, 0.85, 0.88), 0.0));
		std::shared_ptr<Material> light(new DiffuseLight(new ConstTexture(Vec3(1.0, 1.0, 1.0)),15.0));
		std::shared_ptr<Material> glass(new Dielectric(1.5));
		std::shared_ptr<Material> blue_ink(new Isotropic(new ConstTexture(Vec3(0.2, 0.4, 0.9))));
		//wall
		obj_list.push_back(std::make_shared<FlipNormals>(new YZRect(0, 555, 0, 555, 555, red)));
		obj_list.push_back(std::make_shared<YZRect>(0, 555, 0, 555, 0, green));
		obj_list.push_back(std::make_shared<FlipNormals>(new XZRect(0, 555, 0, 555, 555, white)));
		obj_list.push_back(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
		obj_list.push_back(std::make_shared<FlipNormals>(new XYRect(0, 555, 0, 555, 555, white)));

		////box
		obj_list.push_back(std::make_shared<Translate>(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(100, 0, 255)));
		obj_list.push_back(std::make_shared<Translate>(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295)));

		////sphere
		auto boundary = std::make_shared<Translate>(new Sphere(Vec3(0, 0, 0), 100, glass), Vec3(400, 100, 150));
		//important_list.push_back(boundary);
		obj_list.push_back(boundary);

		obj_list.push_back(std::make_shared<ConstantMedium>(boundary, 0.2, blue_ink));
		obj_list.push_back(std::make_shared<Translate>(new Sphere(Vec3(0, 0, 0), 80, glass), Vec3(80, 80, 180)));
		important_list.push_back(std::make_shared<Translate>(new Sphere(Vec3(0, 0, 0), 80, glass), Vec3(80, 80, 180)));

		important_list.push_back(std::make_shared<FlipNormals>(new XZRect(213, 343, 227, 332, 554.5, light)));
		obj_list.push_back(std::make_shared<FlipNormals>(new XZRect(213, 343, 227, 332, 554.5, light)));
	}

};