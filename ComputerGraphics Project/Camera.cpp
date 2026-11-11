#include<gl/glm/glm.hpp> 
#include<gl/glm/gtc/type_ptr.hpp>
#include<gl/glm/gtc/matrix_transform.hpp>
#include"Camera.h"

glm::mat4 Camera::getViewMatrix() const {
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::rotate(mat, yaw, glm::vec3(0.0, 1.0, 0.0));
	mat = glm::rotate(mat, pitch, glm::vec3(1.0, 0.0, 0.0));
	glm::vec3 TransFront =  mat * glm::vec4(front, 0.0f);
	return glm::lookAt(eye, eye + TransFront, up);
}

glm::mat4 Camera::getPerspectiveMatrix(float aspect, float Znear, float Zfar) const
{
	return glm::perspective(fovy, aspect, Znear, Zfar);
}


glm::mat4 Camera::getOrthographicMatrix(float left, float right, float bottom, float top, float Znear, float Zfar) const
{
	return glm::ortho(left, right, bottom, top, Znear, Zfar);
}

void Camera::move(glm::vec3 delta) { eye += delta; }
void Camera::RotatePitch(float delta) { 
	pitch += delta;
}
void Camera::RotateYaw(float delta) { 
	yaw += delta;
}

void Camera::zoom(float delta) {
	float result = fovy + delta;
	if ((delta < 0 and glm::radians(30.f) <= result) or (delta > 0 and result <= glm::radians(150.f)))
		fovy = result;
}