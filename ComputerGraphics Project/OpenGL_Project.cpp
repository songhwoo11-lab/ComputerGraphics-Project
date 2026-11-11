#include "OpenGL.h"
#include"Shader.h"
#include"Camera.h"
#include"Mesh.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "assimp-vc145-mt.lib")

class Model {
public:
	Model() = default;
	Model(std::string_view filename, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 sz = glm::vec3(1.0f), glm::vec3 Color = glm::vec3(0.5f)) : 
		mesh{}, center{ pos }, color{Color}, size{sz},
		pitch{}, yaw{}, roll{}
	{
		mesh.LoadModel(filename);
	}

	void draw(const Shader& shader) const {
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(GLfloat), mesh.vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);

		glUniform3fv(glGetUniformLocation(shader.GetShaderID(), "outColor"), 1, glm::value_ptr(color));
		glUniformMatrix4fv(glGetUniformLocation(shader.GetShaderID(), "world"), 1, GL_FALSE, glm::value_ptr(getWorldMat()));

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	}

	void move(glm::vec3 delta) { center += delta; }
	void SetPos(glm::vec3 pos) { center = pos; }
	glm::vec3 getPos() const { return center; }
	glm::vec3 getsize() const { return size; }
	void rotatePitch(GLfloat delta) { pitch += delta; }
	void rotateYaw(GLfloat delta) { yaw += delta; }
	GLfloat getYaw() const { return yaw; }
		 
private:
 	glm::mat4 getWorldMat() const {
		glm::mat4 worldMat{ glm::mat4(1.0f) };


		worldMat = glm::translate(worldMat, center);
		worldMat = glm::rotate(worldMat, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		worldMat = glm::scale(worldMat, size);

		return worldMat;
	}
	Mesh mesh;
	glm::vec3 center;
	glm::vec3 color;
	glm::vec3 size;
	GLfloat pitch, yaw, roll;
};

namespace {
	Shader shader;

	bool timer = true;
	double fps = 60;
	double dtime = 1 / fps;

	bool isPerspective = true;

	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	std::vector<Model> meshs;

	bool MouseEntered = false;
	double lastX, lastY;
}

void OpenGL::SetBuffer()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void OpenGL::startPractice()
{
	shader = Shader("vertex.glsl", "fragment.glsl");
	EnableState(GL_DEPTH_TEST);
	EnableState(GL_CULL_FACE);
	pCamera = std::make_unique<Camera>(Camera(glm::vec3(0.9, 1.3, -3.1)));
	// 0.9, 1.3, -3.1
	// pitch: -0.17yaw : 0.314533
	lightPos = glm::vec3(5.0f, 0.0f, 0.0f);
	meshs.push_back(Model("Object\\ak47.obj", glm::vec3(), glm::vec3(2.f)));
	glUniform3fv(glGetUniformLocation(shader.GetShaderID(), "viewPos"), 1, glm::value_ptr(pCamera->eye));
	glUniform3fv(glGetUniformLocation(shader.GetShaderID(), "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f)));
	glUniform3fv(glGetUniformLocation(shader.GetShaderID(), "lightPos"), 1, glm::value_ptr(lightPos));
}

void OpenGL::mainloop() {
	while (!glfwWindowShouldClose(window)) {

		if (timer) Timer(dtime);
		rendering();

		glfwPollEvents();
	}
}

// 렌더링 함수
void OpenGL::rendering() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.UseProgram();

	glUniformMatrix4fv(glGetUniformLocation(shader.GetShaderID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetShaderID(), "view"), 1, GL_FALSE, glm::value_ptr(pCamera->getViewMatrix()));
	
	// 그리기
	for (int i = 0; i < meshs.size(); ++i){
		meshs[i].draw(shader);
	}


	glfwSwapBuffers(window);
}

void OpenGL::Timer(double time)
{
	static GLdouble fir = glfwGetTime(), sec;

	sec = glfwGetTime();
	if (sec - fir >= time) {
		fir = sec;
		// 작업 처리

	}
}

void OpenGL::UpdateProjection()
{
	projection = isPerspective ? pCamera->getPerspectiveMatrix(width / (float)height, Znear, Zfar) : pCamera->getOrthographicMatrix(-1., 1, -0.75,0.75, 0.1, 100);
}

// 콜백 함수
void OpenGL::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE) return;
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			MouseEntered = false;
		}
		else {
			glfwSetWindowShouldClose(window, true);
			std::cout << pCamera->eye.x << ", " << pCamera->eye.y << ", " << pCamera->eye.z << '\n';
		}
		break;
	case GLFW_KEY_F1:
		pCamera->move(glm::vec3(0.1f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_F2:
		pCamera->move(glm::vec3(-0.1f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_F3:
		pCamera->move(glm::vec3(0.0f, -0.1f, 0.0f));
		break;
	case GLFW_KEY_F4:
		pCamera->move(glm::vec3(0.0f, 0.1f, 0.0f));
		break;
	case GLFW_KEY_F5:
		pCamera->move(glm::vec3(0.0f, 0.0f, -0.1f));
		break;
	case GLFW_KEY_F6:
		pCamera->move(glm::vec3(0.0f, 0.0f, 0.1f));
		break;
	case GLFW_KEY_F7:
		pCamera->RotateYaw(glm::radians(-1.0f));
		break;
	case GLFW_KEY_F8:
		pCamera->RotateYaw(glm::radians(1.0f));
		break;
	}

}
void OpenGL::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(window, &lastX, &lastY);
		MouseEntered = true;
	}
}
void OpenGL::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
{
	if (MouseEntered) {
		auto dirX = xpos - lastX;
		auto dirY = ypos - lastY;


		float sensitivity = 0.01f;
		pCamera->yaw += sensitivity  * -dirX;
		pCamera->pitch += sensitivity * dirY;
		
		if (pCamera->pitch > glm::radians(89.0f)) pCamera->pitch = glm::radians(89.0f);
		if (pCamera->pitch < glm::radians(-89.0f)) pCamera->pitch = glm::radians(-89.0f);
		std::cout << "pitch: " << pCamera->pitch << "yaw: " << pCamera->yaw << '\n';
		lastX = xpos, lastY = ypos;
	}
}
void OpenGL::CursorEnterCallback(GLFWwindow* window, int entered)
{
	
}
void OpenGL::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

void OpenGL::CharCallback(GLFWwindow* window, unsigned int codepoint)
{
	switch (codepoint) {
	case 'q':
		glfwSetWindowShouldClose(window, true);
		std::cout << "(" << pCamera->eye.x << ", " << pCamera->eye.y << ", " << pCamera->eye.z << ")\n";
		std::cout << pCamera->yaw << '\n';
		break;
	case 'p':
		isPerspective ^= 1;
		UpdateProjection();
	}
}
void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {}
void OpenGL::windowSizeCallback(GLFWwindow* window, int width, int height) { SetViewport(0, 0, width, height); }




