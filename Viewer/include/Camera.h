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

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	void SetOrthographicProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	glm::mat4x4 GetOrthographicProjection();


	//fields additions
	float right, left;
	float top, bottom;
	float zNear, zFar;

	//camera transformation
	glm::mat4x4 c;
	glm::mat4x4 c_inverse;

	//lookat parameters
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

	glm::mat4x4 OrthoProjection;

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;

};
