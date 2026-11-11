#include<iostream>
#include<fstream>
#include<string>
#include<string_view>
#include<gl/glew.h>
#include<gl/glfw3.h>
#include<gl/glm/glm.hpp> 
#include<gl/glm/gtc/type_ptr.hpp>
#include<gl/glm/gtc/matrix_transform.hpp>
#include"Shader.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

Shader::Shader(std::string_view vertexfile, std::string_view fragfile)
{
	makeShader(vertexfile, fragfile);
}

Shader::Shader(Shader&& rhs) noexcept : shaderID{rhs.shaderID}
{
	rhs.shaderID = 0;
}
Shader& Shader::operator=(Shader&& rhs) noexcept
{
	if (shaderID == rhs.shaderID) return *this;
	shaderID = rhs.shaderID;
	rhs.shaderID = 0;
	return *this;
}


Shader::~Shader() 
{
	glDeleteProgram(shaderID);
}

GLuint Shader::CreateShader(std::string_view filename, GLuint WhatShader)
{
	std::string vertexSource = filetobuf(filename);
	const GLchar* vs = vertexSource.c_str();
	GLuint shader = glCreateShader(WhatShader);
	glShaderSource(shader, 1, &vs, NULL);
	glCompileShader(shader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(shader, 512, NULL, errorLog);
		if (WhatShader == GL_VERTEX_SHADER) std::cerr << "vertex\n";
		else std::cerr << "fragment\n";
		std::cerr << "ERROR: 컴파일 실패\n" << errorLog << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

int Shader::makeShader(std::string_view vertexfile, std::string_view fragfile) {

	GLuint vertexShader = CreateShader(vertexfile, GL_VERTEX_SHADER);
	if (not vertexShader) return -1;
	GLuint fragmentShader = CreateShader(fragfile, GL_FRAGMENT_SHADER);
	if (not fragmentShader) return -1;

	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return -1;
	}

	glUseProgram(shaderID);
	return 0;
}

std::string Shader::filetobuf(std::string_view filename) {
	std::ifstream in{ filename.data() };
	if (not in) {
		return "";
	}

	std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	return source;
}
