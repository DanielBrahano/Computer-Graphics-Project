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
	eye = glm::vec3(2.0f, 2.0f, 2.0f);
	at = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 test= glm::lookAt(eye, at, up);

	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			std::cout << test[i][j] << " ";
		}
		std::cout << std::endl;
	}*/
	//std::cout << std::endl;
	//std::cout << std::endl;
	//std::cout << std::endl;
	return glm::lookAt(eye, at, up);
}




