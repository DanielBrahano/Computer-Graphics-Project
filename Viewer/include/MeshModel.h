#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include <glm/gtc/matrix_transform.hpp>//include for mat operations

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	/*---my finction members---*/
	//get the value of #vertex
	float GetVertex(int index, int coordinate);
	//get the value of #normal
	float GetNormal(int index, int coordinate);
	void printObj();
	//print for sanity check
	void ScaleTranslateBunny();
	//functions to translate, scale and rotate WORLD
	void MeshModel::WorldTranslateModel(float x, float y, float z);
	void MeshModel::WorldScaleModel(float x, float y, float z);
	void MeshModel::WorldRotateModel(float angle, glm::vec3 axis);
	//functions to translate, scale and rotate MODEL
	void MeshModel::ObjectTranslateModel(float x, float y, float z);
	void MeshModel::ObjectScaleModel(float x, float y, float z);
	void MeshModel::ObjectRotateModel(float angle, glm::vec3 axis);
	//get the total transformation as seen in the class
	glm::mat4x4 GetTransform() {
		return   worldTransform * objectTransform;
	}

	//each model contains its own transformations
	glm::mat4 objectTransform;
	glm::mat4 worldTransform;
	glm::mat4 objectTranslate;
	glm::mat4 objectRotate;
	glm::mat4 objectScale;
	glm::mat4 worldTranslate;
	glm::mat4 worldRotate;
	glm::mat4 worldScale;

	bool DrawWorldAxes;
	bool DrawModelAxes;

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;


};
