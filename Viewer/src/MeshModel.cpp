#include "MeshModel.h"

#include <iostream>
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{
 objectTransform = glm::mat4(1.0f);
 worldTransform = glm::mat4(1.0f);
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
	//objectTransform = glm::mat4(1.0f);


	WorldTranslateModel(600, 400, 300);
	WorldScaleModel(250, 250,250 );
	ObjectRotateModelZ(9, glm::vec3(1.0f, 1.0f, 1.0f));


}
void MeshModel::WorldTranslateModel(float x, float y, float z)
{
	worldTransform = glm::translate(worldTransform, { x,y,z });
}
void MeshModel::WorldScaleModel(float x, float y, float z)
{
	worldTransform = glm::scale(worldTransform, { x,y,z });
	
}

void MeshModel::WorldRotateModelZ(float angle,  glm::vec3 axis)
{
	worldTransform = glm::rotate(worldTransform, glm::radians(angle), axis);
}

void MeshModel::ObjectTranslateModel(float x, float y, float z)
{
	objectTransform = glm::translate(objectTransform, { x,y,z });
}
void MeshModel::ObjectScaleModel(float x, float y, float z)
{
	objectTransform = glm::scale(objectTransform, { x,y,z });

}

void MeshModel::ObjectRotateModelZ(float angle, glm::vec3 axis)
{
	objectTransform = glm::rotate(objectTransform, glm::radians(angle), axis);
}


