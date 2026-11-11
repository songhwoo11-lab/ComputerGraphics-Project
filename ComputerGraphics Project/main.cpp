#include<iostream>
#include<gl/glew.h>
#include<gl/glfw3.h>
#include"OpenGL.h"

OpenGL opengl;

int main() {
	int err = opengl.init_OpenGL("Agent Rescue Project");
	if (err) return err;

	opengl.SetCallBack();
	opengl.SetBuffer();
	opengl.startPractice();
	opengl.mainloop();
}