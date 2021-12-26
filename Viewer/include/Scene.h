#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Light.h"


using namespace std;

class Scene {
public:
	Scene();

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;

	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	void Scene::AddLight(Light* light);
	Light& Scene::GetLight(int index);

	void SetActiveLightIndex(int index);
	int GetActiveLightIndex() const;

	Light& GetActiveLight();

	bool draw_box;
	bool draw_normals;
	bool draw_face_normals;
	bool bounding_rectangles;
	bool paint_triangles;
	bool gray_scale;
	bool color_with_buffer;

	//--------------move to light class-------------------move to light class----------------------move to light class---------------------move to light class---------------------
	bool lighting;
	bool ambient_shading;
	bool flat_shading;
	//--------------move to light class-------------------move to light class----------------------move to light class---------------------move to light class---------------------
	


private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	vector<Light*> lights;


	int active_camera_index;
	int active_model_index;
	int active_light_index;
};