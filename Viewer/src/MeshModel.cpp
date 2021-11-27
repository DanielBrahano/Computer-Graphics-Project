#include "MeshModel.h"

#include <iostream>
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{
	//initialize all matrices to identity
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
}

MeshModel::~MeshModel()
{
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
	WorldScaleModel(200, 200,200 );
	
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
void MeshModel::WorldRotateModel(float angle,  glm::vec3 axis)
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



