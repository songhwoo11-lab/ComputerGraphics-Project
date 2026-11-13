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
	glm::vec3 getPos() const { return center; }
	void SetPos(glm::vec3 pos) { center = pos; }

	glm::vec3 getsize() const { return size; }

	void rotatePitch(GLfloat delta) { pitch += delta; }
	GLfloat getPitch() const { return pitch; }
	void SetPitch(GLfloat newPth) { pitch = newPth; }

	void rotateYaw(GLfloat delta) { yaw += delta; }
	GLfloat getYaw() const { return yaw; }
	void SetYaw(GLfloat newYaw) { yaw = newYaw; }
		 
private:
 	glm::mat4 getWorldMat() const {
		glm::mat4 worldMat{ glm::mat4(1.0f) };


		worldMat = glm::translate(worldMat, center);
		worldMat = glm::rotate(worldMat, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		worldMat = glm::rotate(worldMat, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
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

	Model ak47;
	glm::vec3 pos = glm::vec3(-0.466667, -0.4, 0.8);

	bool MouseEntered = false;
	double lastX, lastY;

	bool move = false;
	float frontState, leftState;
	constexpr float meterpersec = 1.94f;
}

namespace {
	void drawFloor(const Shader& shader) {
		static glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);
		GLfloat vertices[] = {
			50.0f, 0.0f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-50.0f, 0.0f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		};
		GLuint indices[] = {
			0,1,2,
			0,2,3,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glUniform3fv(glGetUniformLocation(shader.GetShaderID(), "outColor"), 1, glm::value_ptr(color));
		glUniformMatrix4fv(glGetUniformLocation(shader.GetShaderID(), "world"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	}
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
	pCamera = std::make_unique<Camera>(Camera(glm::vec3(0.0, 2.0, 0.0)));

	lightPos = glm::vec3(0.0f, 5.0f, 0.0f);
	ak47 = Model("Object\\ak47.obj", pCamera->eye + pos, glm::vec3(0.5f));
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
	glClearColor(0.5f, 0.5f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.UseProgram();

	glUniform3fv(glGetUniformLocation(shader.GetShaderID(), "viewPos"), 1, glm::value_ptr(pCamera->eye));
	glUniform3fv(glGetUniformLocation(shader.GetShaderID(), "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f)));
	glUniform3fv(glGetUniformLocation(shader.GetShaderID(), "lightPos"), 1, glm::value_ptr(lightPos));

	glUniformMatrix4fv(glGetUniformLocation(shader.GetShaderID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetShaderID(), "view"), 1, GL_FALSE, glm::value_ptr(pCamera->getViewMatrix()));
	
	// ak47를 화면에 고정시키기
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::rotate(mat, ak47.getYaw(), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, ak47.getPitch(), glm::vec3(1.0f, 0.0f, 0.0f));
	ak47.SetPos(pCamera->eye + glm::vec3(mat * glm::vec4(pos, 1.0f)));

	// 그리기
	drawFloor(shader);
	ak47.draw(shader);


	glfwSwapBuffers(window);
}

void OpenGL::Timer(double time)
{
	static GLdouble fir = glfwGetTime(), sec;

	sec = glfwGetTime();
	if (sec - fir >= time) {
		fir = sec;
		// 작업 처리	
		auto front = pCamera->getFront();
		pCamera->eye += frontState * glm::vec3(front.x, 0, front.z);
		front = glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(front, 1.0f);
		pCamera->eye += leftState * glm::vec3(front.x, 0, front.z);
	}
}

void OpenGL::UpdateProjection()
{
	projection = isPerspective ? pCamera->getPerspectiveMatrix(width / (float)height, Znear, Zfar) : pCamera->getOrthographicMatrix(-1., 1, -0.75,0.75, 0.1, 100);
}

// 콜백 함수
void OpenGL::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_RELEASE) return;
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			MouseEntered = false;
		}
		else {
			glfwSetWindowShouldClose(window, true);
			std::cout << pCamera->eye.x << ", " << pCamera->eye.y << ", " << pCamera->eye.z << '\n';
		}
		break;
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)
			frontState += 1.0f;
		if (action == GLFW_RELEASE)
			frontState -= 1.0f;
			
		break;
	case GLFW_KEY_A:
		if (action == GLFW_PRESS)
			leftState += meterpersec;
		if (action == GLFW_RELEASE)
			leftState -= meterpersec;
		break;
	case GLFW_KEY_S:
		if (action == GLFW_PRESS)
			frontState -= meterpersec;
		if (action == GLFW_RELEASE)
			frontState += meterpersec;
		break;
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
			leftState -= meterpersec;
		if (action == GLFW_RELEASE)
			leftState += meterpersec;
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


		float sensitivity = 0.003f;
		pCamera->yaw += sensitivity  * -dirX;
		pCamera->pitch += sensitivity * dirY;

		if (pCamera->pitch > glm::radians(89.0f)) pCamera->pitch = glm::radians(89.0f);
		if (pCamera->pitch < glm::radians(-89.0f)) pCamera->pitch = glm::radians(-89.0f);

		ak47.SetPitch(pCamera->pitch);
		ak47.SetYaw(pCamera->yaw);
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
		break;

	}
}
void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {}
void OpenGL::windowSizeCallback(GLFWwindow* window, int width, int height) { SetViewport(0, 0, width, height); }




