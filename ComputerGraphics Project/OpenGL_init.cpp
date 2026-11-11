#include "OpenGL.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

OpenGL::~OpenGL() {
	//--- 종료 처리
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "glfw end" << std::endl;
}

int OpenGL::init_OpenGL(std::string_view title) {
	//--- GLFW 초기화
	if (!glfwInit()) {
		std::cerr << "failed initializing GLFW!" << std::endl;
		return -1;
	}
	std::cout << "completed initializing GLFW" << std::endl;

	//--- OpenGL 버전 설정
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//--- 윈도우생성
	window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	if (not window) {
		std::cerr << "failed creating window" << std::endl;
		glfwTerminate();
		return -2;
	}
	std::cout << "completed creating window" << std::endl;
	//--- 컨텍스트설정
	glfwMakeContextCurrent(window);

	//--- GLEW 초기화
	glewExperimental = GL_TRUE; // 최신 기능 사용
	if (glewInit() != GLEW_OK) {
		std::cerr << "failed initializing GLEW!" << std::endl;
		return -3;
	}
	std::cout << "completed initializing GLEW" << std::endl;
	return 0;
}
void OpenGL::EnableState(GLenum state) const
{
	glEnable(state);
}
void OpenGL::DisableState(GLenum state) const
{
	glEnable(state);
}
GLboolean OpenGL::IsEnabledState(GLenum state) const
{
	return glIsEnabled(state);
}
 

void OpenGL::SetCallBack()
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, keyboardCallbackEntry);
	glfwSetMouseButtonCallback(window, MouseButtonCallbackEntry);
	glfwSetCursorPosCallback(window, CursorPosCallbackEntry);
	glfwSetCursorEnterCallback(window, CursorEnterCallbackEntry);
	glfwSetScrollCallback(window, ScrollCallbackEntry);
	glfwSetCharCallback(window, CharCallbackEntry);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallbackEntry);
	glfwSetWindowSizeCallback(window, windowSizeCallbackEntry);
}

void OpenGL::keyboardCallbackEntry(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	OpenGL* gl = reinterpret_cast<OpenGL*>(glfwGetWindowUserPointer(window));
	if(gl) gl->keyboardCallback(window, key, scancode, action, mods);
}
void OpenGL::MouseButtonCallbackEntry(GLFWwindow* window, int button, int action, int mods) {
	OpenGL* gl = reinterpret_cast<OpenGL*>(glfwGetWindowUserPointer(window));
	if (gl) gl->MouseButtonCallback(window, button, action, mods);
}

void OpenGL::CursorPosCallbackEntry(GLFWwindow* window, double xpos, double ypos) {
	OpenGL* gl = reinterpret_cast<OpenGL*>(glfwGetWindowUserPointer(window));
	if (gl) gl->CursorPosCallback(window,xpos, ypos);
}
void OpenGL::CursorEnterCallbackEntry(GLFWwindow* window, int entered) {
	OpenGL* gl = reinterpret_cast<OpenGL*>(glfwGetWindowUserPointer(window));
	if (gl) gl->CursorEnterCallback(window, entered);
}
void OpenGL::ScrollCallbackEntry(GLFWwindow* window, double xoffset, double yoffset) {
	OpenGL* gl = reinterpret_cast<OpenGL*>(glfwGetWindowUserPointer(window));
	if (gl) gl->ScrollCallback(window, xoffset, yoffset);
}

void OpenGL::CharCallbackEntry(GLFWwindow* window, unsigned int codepoint) {
	OpenGL* gl = reinterpret_cast<OpenGL*>(glfwGetWindowUserPointer(window));
	if (gl) gl->CharCallback(window, codepoint);
}

void OpenGL::FramebufferSizeCallbackEntry(GLFWwindow* window, int width, int height) {
	OpenGL* gl = reinterpret_cast<OpenGL*>(glfwGetWindowUserPointer(window));
	if (gl) gl->FramebufferSizeCallback(window, width, height);
}

void OpenGL::windowSizeCallbackEntry(GLFWwindow* window, int width, int height) {
	OpenGL* gl = reinterpret_cast<OpenGL*>(glfwGetWindowUserPointer(window));
	if (gl) gl->windowSizeCallback(window, width, height);
}

