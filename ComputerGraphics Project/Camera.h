#pragma once

struct Camera {
public:
	Camera(glm::vec3 eye = glm::vec3(0.0f, 0.0f, -20.0f), 
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
			glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f), 
			float fovy = glm::radians(90.f)
			)
		: eye {eye}, up{ up }, front{front}, at{glm::normalize(front - eye)},
		fovy{ fovy }, pitch{}, yaw{} {}
	glm::mat4 getViewMatrix() const;
	glm::mat4 getPerspectiveMatrix(float aspect, float Znear, float Zfar) const;
	glm::mat4 getOrthographicMatrix(float left, float right, float bottom, float top, float Znear, float Zfar) const;
	void move(glm::vec3 delta);
	void RotatePitch(float delta);
	void RotateYaw(float delta);
	void zoom(float delta);
	glm::vec3 getFront() const;
public:
	glm::vec3 eye, up;
	glm::vec3 front;
	glm::vec3 at;

	float fovy;
	float pitch;
	float yaw;
};
