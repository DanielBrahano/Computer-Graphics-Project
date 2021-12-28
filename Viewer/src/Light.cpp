#include "Light.h"
#include <iostream>

Light::Light()
{

	AmbientColor = glm::vec3(1, 1, 1);//La
	DiffuseColor = glm::vec3(1, 1, 1);//Ld
	SpecularColor = glm::vec3(1, 1, 1);//Ls

	Ia = glm::vec3(0.f, 0.f, 0.f);
	Id = glm::vec3(0.f, 0.f, 0.f);
	Is = glm::vec3(0.f, 0.f, 0.f);
	Translation = glm::mat4(1.0f);

	Translation = glm::translate(Translation, { 1,1,1 });

}

void Light::Translate(float x, float y, float z)
{
	Translation = glm::translate(Translation, { x,y,z });

}

glm::vec3 Light::Compute_Ia(glm::vec3 Ka)
{
	//Ia = La * K*a
	Ia = glm::vec3(Ka.x * AmbientColor.x, Ka.y * AmbientColor.y, Ka.z * AmbientColor.z);
	return Ia;
}

glm::vec3 Light::Compute_Id(glm::vec3 Kd)
{
	//Id=Kd*(I*n)Ld
	Id = glm::vec3(Kd.x * DiffuseColor.x, Kd.y * DiffuseColor.y, Kd.z * DiffuseColor.z);
	Id = glm::normalize(Id);
	float theta  = glm::dot(N,I);
	return glm::vec3(theta * Id.x, theta * Id.y, theta * Id.z);
}

glm::vec3 Light::GetPosition()
{
	glm::vec3 position = { Translation[3].x,Translation[3].y ,Translation[3].z };
	return position;
}

glm::vec3 Light::GetLight()
{
	glm::vec3 color=glm::vec3(Ia.x + Id.x + Is.x, Ia.y + Id.y + Is.y, Ia.z + Id.z + Is.z);
	return color;
}
