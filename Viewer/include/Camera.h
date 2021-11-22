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

	//transformations functions with incremental 
	void WorldTranslate(float x, float y, float z);
	void LocalTranslate(float x, float y, float z);
	void WorldScale(float x, float y, float z);
	void LocalScale(float x, float y, float z);
	void LocalRotate(float angle, glm::vec3 axis);
	void WorldRotate(float angle, glm::vec3 axis);

	glm::mat4x4 GetCameraTransform();
	glm::mat4x4 GetInverseCameraTransform();


	//fields additions
	float right, left;
	float top, bottom;
	float _near, _far;

	//camera transformations
	glm::mat4x4 localTransform;
	glm::mat4x4 worldTransform;
	glm::mat4x4 localTranslate;
	glm::mat4x4 localRotate;
	glm::mat4x4 localScale;
	glm::mat4x4 worldTranslate;
	glm::mat4x4 worldRotate;
	glm::mat4x4 worldScale;

	//inverse transformations
	glm::mat4x4 inv_localTransform;
	glm::mat4x4 inv_worldTransform;
	glm::mat4x4 inv_localTranslate;
	glm::mat4x4 inv_localRotate;
	glm::mat4x4 inv_localScale;
	glm::mat4x4 inv_worldTranslate;
	glm::mat4x4 inv_worldRotate;
	glm::mat4x4 inv_worldScale;

	//camera transformation
	glm::mat4x4 c;
	glm::mat4x4 c_inverse;

	//lookat parameters
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;

};
