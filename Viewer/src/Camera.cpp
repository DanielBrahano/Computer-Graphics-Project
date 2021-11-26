#include "Camera.h"

Camera::Camera()
{
	view_transformation = glm::mat4x4(1.0f);
	projection_transformation = glm::mat4x4(1.0f);
	//initiallize all new variables/transform matrices
	right = 1280.0f;
	left = 0.0f;
	top = 720.0f;
	bottom = 0.0f;
	zNear = 0.0f;
	zFar = 0.0f;

	eye = glm::vec3(1.0f, 1.0f, 1.0f);
	at = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4x4 OrthoProjection = glm::mat4x4(1.0f);
	glm::mat4x4 viewTransformation = glm::mat4x4(1.0f);//glm::lookAt(eye, at, up);
	glm::mat4x4 test = glm::mat4x4(1.0f);//glm::lookAt(eye, at, up);


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
	OrthoProjection = glm::ortho(left, right, bottom, top, zNear, zFar);
}

glm::mat4x4 Camera::GetOrthographicProjection()
{
	return glm::inverse(OrthoProjection);
}

glm::mat4x4 Camera::GetViewTransformation()
{
	return viewTransformation;
}




