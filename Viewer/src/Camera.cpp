#include "Camera.h"

Camera::Camera()
{
	view_transformation = glm::mat4x4(1.0f);
	projection_transformation = glm::mat4x4(1.0f);
	//initiallize all new variables/transform matrices
	right = 0.0f;
	left = 0.0f;
	top = 0.0f;
	bottom = 0.0f;
	zNear = 0.0f;
	zFar = 0.0f;

	eye = glm::vec3(1.0f, 1.0f, 1.0f);
	at = glm::vec3(0.0f, 0.0f, 0.0f);
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
	

	/*std::cout << "Ortho:" << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			std::cout << proj[i][j] << " ";
		}
		std::cout << std::endl;
	}*/
}

void Camera::SetPerspectiveProjection(const float fovy, const float aspectRatio, const float zNear, const float zFar)
{
	//glm::mat4 temp = glm::perspective(fovy, aspectRatio, zNear, zFar);
	//proj = glm::inverse(temp);

	proj= glm::perspective(fovy, aspectRatio, zNear, zFar);
	
	std::cout << "Pres:" << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			std::cout << proj[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
glm::mat4x4 Camera:: GetProjectionTransformation()
{
	return proj;
}

glm::mat4x4 Camera::GetViewTransformation()
{
	return viewTransformation;
}




