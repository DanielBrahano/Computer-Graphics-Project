#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>//include for mat operations
#include <iostream>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);


	void SetOrthographicProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetPerspectiveProjection(const float fovy, const float aspectRatio, const float zNear, const float zFar);
	glm::mat4x4 GetProjectionTransformation();
	glm::mat4x4 GetViewTransformation();


	//fields additions
	float right, left;
	int top, bottom;
	float zNear, zFar;
	float fovy;
	float aspectRatio;
	

	//lookat parameters
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

	glm::mat4x4 viewTransformation;
	glm::mat4x4 test;
	glm::mat4x4 proj;

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;

};