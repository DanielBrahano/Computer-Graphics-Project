#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include "Face.h"
#include <glm/gtc/matrix_transform.hpp>//include for mat operations

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
public:
	MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	/*---my function members---*/
	int getVerticesSize() {
		return vertices.size();
	}
	//get the value of #vertex
	float GetVertex(int index, int coordinate);
	//get the value of #normal
	float GetNormal(int index, int coordinate);
	//get normals vec
	std::vector<glm::vec3> GetNormals();
	void printObj();
	//print for sanity check
	void ScaleTranslateBunny();
	//functions to translate, scale and rotate WORLD
	void WorldTranslateModel(float x, float y, float z);
	void WorldScaleModel(float x, float y, float z);
	void WorldRotateModel(float angle, glm::vec3 axis);
	//functions to translate, scale and rotate MODEL
	void ObjectTranslateModel(float x, float y, float z);
	void ObjectScaleModel(float x, float y, float z);
	void ObjectRotateModel(float angle, glm::vec3 axis);
	//get the total transformation as seen in the class
	glm::mat4x4 GetTransform() {
		return   worldTransform * objectTransform;
	}

	void ResetTransformations();

	glm::vec3 MeshModel::GetPosition();

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

	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;

	glm::vec3 color;
	std::vector<Vertex> modelVertices;

	GLuint vbo;
	GLuint vao;

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;




};
