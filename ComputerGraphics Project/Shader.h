#pragma once

class Shader {
public:
	Shader() = default;
	Shader(std::string_view vertexfile, std::string_view fragfile);
	~Shader();

	Shader(const Shader& rhs) = delete;
	Shader& operator=(const Shader& rhs) = delete;

	Shader(Shader&& rhs) noexcept;
	Shader& operator=(Shader&& rhs) noexcept;

	GLuint GetShaderID() const { return shaderID; }
	void UseProgram() const { glUseProgram(shaderID); }
private:
	GLuint shaderID = 0;

	int makeShader(std::string_view vertexfile, std::string_view fragfile);
	GLuint CreateShader(std::string_view file, GLuint whatShader);
	std::string filetobuf(std::string_view filename);
};