#include "Camera.h"

Camera::Camera()
{
	view_transformation = glm::mat4x4(1.0f);
	projection_transformation = glm::mat4x4(1.0f);
	//initiallize all new variables/transform matrices
	right = 1;
	left = -1;
	top = 1.0f;
	bottom = -1.0f;
	zNear = -1.0f;
	zFar = 1.0f;
	fovy = 45.0f;
	aspectRatio = 1.0;

	eye = glm::vec3(0.0f, 0.0f, 1.0f);
	at = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4x4 proj = glm::mat4x4(1.0f);

	glm::mat4x4 viewTransformation = glm::mat4x4(1.0f);
	glm::mat4x4 test = glm::mat4x4(1.0f);


}

Camera::~Camera()
{
	
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	viewTransformation = glm::lookAt(eye, at, up);
}

void Camera::SetOrthographicProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{	
	proj = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void Camera::SetPerspectiveProjection(const float fovy, const float aspectRatio, const float zNear, const float zFar)
{
	proj= glm::perspective(fovy, aspectRatio, zNear, zFar);
}
glm::mat4x4 Camera:: GetProjectionTransformation()
{
	return proj;
}

glm::mat4x4 Camera::GetViewTransformation()
{
	return viewTransformation;
}





