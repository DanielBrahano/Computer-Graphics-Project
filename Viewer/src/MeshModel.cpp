#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	textureCoords(textureCoords),
	model_name(modelName)
{
	objectTransform = glm::mat4(1.0f);
	worldTransform = glm::mat4(1.0f);
	objectTranslate = glm::mat4(1.0f);
	objectRotate = glm::mat4(1.0f);
	objectScale = glm::mat4(1.0f);
	worldTranslate = glm::mat4(1.0f);
	worldRotate = glm::mat4(1.0f);
	worldScale = glm::mat4(1.0f);

	DrawWorldAxes = false;
	DrawModelAxes = false;

	Ka = glm::vec3(0.0f, 0, 0);
	Kd = glm::vec3(1, 0, 0);
	Ks = glm::vec3(1.0f, 1, 1);

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	color = glm::vec3(dist(mt), dist(mt), dist(mt));

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int normalIndex = currentFace.GetNormalIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}

			if (normals.size() > 0) {
				vertex.normal = normals[normalIndex];
			}

			modelVertices.push_back(vertex);
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

float MeshModel::GetVertex(int index, int coordinate)
{
	return vertices[index][coordinate];
}

float MeshModel::GetNormal(int index, int coordinate)
{
	return normals[index][coordinate];
}

std::vector<glm::vec3> MeshModel::GetNormals()
{
	return normals;
}

void MeshModel::printObj()
{
	int faceCounts = GetFacesCount();
	int maxVertex = 0, maxNormal = 0;
	for (int i = 0; i < faceCounts; i++)
	{
		//find the number of vertices in the model
		if (GetFace(i).GetVertexIndex(0) > maxVertex)
			maxVertex = GetFace(i).GetVertexIndex(0);
		if (GetFace(i).GetVertexIndex(1) > maxVertex)
			maxVertex = GetFace(i).GetVertexIndex(1);
		if (GetFace(i).GetVertexIndex(2) > maxVertex)
			maxVertex = GetFace(i).GetVertexIndex(2);

		//find the number of normals in the model
		if (GetFace(i).GetNormalIndex(0) > maxNormal)
			maxNormal = GetFace(i).GetNormalIndex(0);
		if (GetFace(i).GetNormalIndex(1) > maxNormal)
			maxNormal = GetFace(i).GetNormalIndex(1);
		if (GetFace(i).GetNormalIndex(2) > maxNormal)
			maxNormal = GetFace(i).GetNormalIndex(2);



	}
	//loop to print all the vertices 
	for (int i = 0; i < maxVertex; i++) {
		std::cout << "v ";
		std::cout << GetVertex(i, 0) << " ";
		std::cout << GetVertex(i, 1) << " ";
		std::cout << GetVertex(i, 2) << std::endl;
	}
	std::cout << "# verticies " << maxVertex << std::endl;

	//loop to print all the normals 
	for (int i = 0; i < maxNormal; i++) {
		std::cout << "vn ";
		std::cout << GetNormal(i, 0) << " ";
		std::cout << GetNormal(i, 1) << " ";
		std::cout << GetNormal(i, 2) << std::endl;
	}
	std::cout << "# normals " << maxNormal << std::endl;

	for (int i = 0; i < faceCounts; i++)
	{
		std::cout << "f ";
		for (int j = 0; j < 3; j++) {
			std::cout << GetFace(i).GetVertexIndex(j) << "//" << GetFace(i).GetNormalIndex(j) << " ";
		}
		std::cout << " " << std::endl;
	}
}

//for number 2, were are asked to scale and translate small mesh so i chose bunny, first translate by 380 each coordinate and then scale by 200
void MeshModel::ScaleTranslateBunny()
{
	worldTransform = glm::mat4(1.0f);
	objectTransform = glm::mat4(1.0f);

	WorldTranslateModel(600, 400, 300);
	WorldScaleModel(200, 200, 200);

}
void MeshModel::WorldTranslateModel(float x, float y, float z)
{
	worldTranslate = glm::translate(worldTranslate, { x,y,z });
	//update world transform matrix
	worldTransform = worldTranslate * worldRotate * worldScale;

}
void MeshModel::WorldScaleModel(float x, float y, float z)
{
	worldScale = glm::scale(worldScale, { x,y,z });
	//update world transform matrix
	worldTransform = worldTranslate * worldRotate * worldScale;
}
void MeshModel::WorldRotateModel(float angle, glm::vec3 axis)
{
	worldRotate = glm::rotate(worldRotate, glm::radians(angle), axis);
	//update world transform matrix
	worldTransform = worldTranslate * worldRotate * worldScale;
}
void MeshModel::ObjectTranslateModel(float x, float y, float z)
{
	objectTranslate = glm::translate(objectTranslate, { x,y,z });
	//update object transform matrix
	objectTransform = objectTranslate * objectRotate * objectScale;
}
void MeshModel::ObjectScaleModel(float x, float y, float z)
{
	objectScale = glm::scale(objectScale, { x,y,z });
	//update object transform matrix
	objectTransform = objectTranslate * objectRotate * objectScale;
}
void MeshModel::ObjectRotateModel(float angle, glm::vec3 axis)
{
	objectRotate = glm::rotate(objectRotate, glm::radians(angle), axis);
	//update objec ttransform matrix
	objectTransform = objectTranslate * objectRotate * objectScale;
}

//reset tansformations
void MeshModel::ResetTransformations()
{
	objectTransform = glm::mat4(1.0f);
	worldTransform = glm::mat4(1.0f);
	objectTranslate = glm::mat4(1.0f);
	objectRotate = glm::mat4(1.0f);
	objectScale = glm::mat4(1.0f);
	worldTranslate = glm::mat4(1.0f);
	worldRotate = glm::mat4(1.0f);
	worldScale = glm::mat4(1.0f);
}

glm::vec3 MeshModel::GetPosition()
{
	glm::mat4 hi = GetTransform();
	glm::vec3 position = { GetTransform()[3].x,GetTransform()[3].y ,GetTransform()[3].z };

	return position;
}

GLuint MeshModel::GetVAO() const
{
	return vao;
}

const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}


void MeshModel::SetPlane()
{
	for (int i = 0; i < modelVertices.size(); i++)
	{
		modelVertices.at(i).textureCoords.x = modelVertices.at(i).position.x;
		modelVertices.at(i).textureCoords.y = modelVertices.at(i).position.y;
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_VERTEX_ARRAY, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, modelVertices.size() * sizeof(Vertex), &modelVertices[0]);
	glBindVertexArray(0);
}

