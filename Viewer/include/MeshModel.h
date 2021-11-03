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
	//functions so translate, scale androtate
	void MeshModel::TranslateModel(float x, float y, float z);
	void MeshModel::ScaleModel(float x, float y, float z);
	void MeshModel::RotateModel(float x, float y, float z);

	//each model contains its own transformations
	glm::mat4 objectTransform;
	glm::mat4 worldTransform;


private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;


};
