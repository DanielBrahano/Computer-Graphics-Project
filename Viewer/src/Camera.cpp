#include "Camera.h"

Camera::Camera()
{
	view_transformation = glm::mat4(1.0f);
	projection_transformation = glm::mat4(1.0f);
	//initiallize all new variables/transform matrices
	right = 1280.0f;
	left = 0.0f;
	top = 720.0f;
	bottom = 0.0f;
	_near = 0.0f;
	_far = 0.0f;

	//transformations
	localTransform = glm::mat4(1.0f);
	worldTransform = glm::mat4(1.0f);
	localTranslate = glm::mat4(1.0f);
	localRotate = glm::mat4(1.0f);
	localScale = glm::mat4(1.0f);
	worldTranslate = glm::mat4(1.0f);
	worldRotate = glm::mat4(1.0f);
	worldScale = glm::mat4(1.0f);

	//inverse transformations
	inv_localTransform = glm::mat4(1.0f);
	inv_worldTransform = glm::mat4(1.0f);
	inv_localTranslate = glm::mat4(1.0f);
	inv_localRotate = glm::mat4(1.0f);
	inv_localScale = glm::mat4(1.0f);
	inv_worldTranslate = glm::mat4(1.0f);
	inv_worldRotate = glm::mat4(1.0f);
	inv_worldScale = glm::mat4(1.0f);

	c = glm::mat4(1.0f);
	c_inverse = glm::mat4(1.0f);
}

Camera::~Camera()
{
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}


void Camera::WorldTranslate(float x, float y, float z)
{
	worldTranslate = glm::translate(worldTranslate, { x,y,z });
	//update world transform matrix
	worldTransform = worldTranslate * worldRotate * worldScale;
	//calculate inverse world transform matrix
	inv_worldTranslate = glm::inverse(worldTranslate);
	inv_worldTransform = inv_worldScale * inv_worldRotate * inv_worldTranslate;

}

void Camera::LocalTranslate(float x, float y, float z)
{
	localTranslate = glm::translate(localTranslate, { x,y,z });
	//update object transform matrix
	localTransform = localTranslate * localRotate * localScale;
	//calculate inverse local tramsform matrix
	inv_localTranslate = glm::inverse(localTranslate);
	inv_localTransform = inv_localScale * inv_localRotate * inv_localTranslate;

}

void Camera::WorldScale(float x, float y, float z)
{
	worldScale = glm::scale(worldScale, { x,y,z });
	//update world transform matrix
	worldTransform = worldTranslate * worldRotate * worldScale;
	//calculate inverse world transform matrix
	inv_worldScale = glm::inverse(worldScale);
	inv_worldTransform = inv_worldScale * inv_worldRotate * inv_worldTranslate;
}

void Camera::LocalScale(float x, float y, float z)
{
	localScale = glm::scale(localScale, { x,y,z });
	//update local transform matrix
	localTransform = localTranslate * localRotate * localScale;
	inv_localTransform = inv_localScale * inv_localRotate * inv_localTranslate;
}

void Camera::WorldRotate(float angle, glm::vec3 axis)
{
	worldRotate = glm::rotate(worldRotate, glm::radians(angle), axis);
	//update world transform matrix
	worldTransform = worldTranslate * worldRotate * worldScale;
	//calculate inverse world transform matrix
	inv_worldRotate = glm::inverse(worldRotate);
	inv_worldTransform = inv_worldScale * inv_worldRotate * inv_worldTranslate;
}

void Camera::LocalRotate(float angle, glm::vec3 axis)
{
	localRotate = glm::rotate(localRotate, glm::radians(angle), axis);
	//update local transform matrix
	localTransform = localTranslate * localRotate * localScale;
	inv_localTransform = inv_localScale * inv_localRotate * inv_localTranslate;
}

//get camera and inverse camera transformations
glm::mat4x4 Camera::GetCameraTransform()
{
	c = worldTransform * localTransform;
}

glm::mat4x4 Camera::GetInverseCameraTransform()
{
	c_inverse = inv_localTransform * inv_worldTransform;
}




