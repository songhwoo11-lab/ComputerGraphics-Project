#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<random>
#include<deque>
#include<algorithm>
#include<gl/glew.h>
#include<gl/glfw3.h>
#include<gl/glm/glm.hpp> 
#include<gl/glm/gtc/type_ptr.hpp>
#include<gl/glm/gtc/matrix_transform.hpp>
#include<assimp/scene.h>
#include<assimp/Importer.hpp>
#include<assimp/postprocess.h> 
#include"Camera.h"




class OpenGL {
public:
	OpenGL() : window{ nullptr }, width{ 1200 }, height{ 800 }, 
		projection{ glm::perspective(glm::radians(90.f), width / (float)height, 0.1f, 1000.f)}
		, Znear{ 0.1f }, Zfar{ 1000.f } {}
	~OpenGL();
	int init_OpenGL(std::string_view title);
	void SetViewport(GLint x, GLint y, GLsizei w, GLsizei h) { 
		glViewport(x, y, w, h); width = w; height = h; 
		UpdateProjection();
	}
	void SetBuffer();
	void mainloop();
	void SetCallBack();
	void startPractice();

	void EnableState(GLenum state) const;
	void DisableState(GLenum state) const;
	GLboolean IsEnabledState(GLenum state) const;
private:
	GLFWwindow* window;
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	std::unique_ptr<Camera> pCamera;
	int width, height;
	glm::mat4 projection;
	GLfloat Znear, Zfar;

	void UpdateProjection();

	void rendering();
	void Timer(double);

	static void keyboardCallbackEntry(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallbackEntry(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCallbackEntry(GLFWwindow* window, double xpos, double ypos);
	static void CursorEnterCallbackEntry(GLFWwindow* window, int entered);
	static void ScrollCallbackEntry(GLFWwindow* window, double xoffset, double yoffset);
	static void CharCallbackEntry(GLFWwindow* window, unsigned int codepoint);
	static void FramebufferSizeCallbackEntry(GLFWwindow* window, int width, int height);
	static void windowSizeCallbackEntry(GLFWwindow* window, int width, int height);

	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void CursorEnterCallback(GLFWwindow* window, int entered);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void CharCallback(GLFWwindow* window, unsigned int codepoint);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void windowSizeCallback(GLFWwindow* window, int width, int height);
};


