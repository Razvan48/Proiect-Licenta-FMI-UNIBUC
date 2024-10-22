#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Renderer
{
private:
	Renderer();
	~Renderer();
	Renderer(const Renderer& other) = delete;
	Renderer& operator= (const Renderer& other) = delete;
	Renderer(const Renderer&& other) = delete;
	Renderer& operator= (const Renderer&& other) = delete;

	const std::string VERTEX_SHADER_PATH;
	const std::string FRAGMENT_SHADER_PATH;

	std::string readShader(const std::string& shaderPath);
	const std::string vertexShaderSource;
	const std::string fragmentShaderSource;

	GLuint shaderProgram;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLint projectionMatrixLocation;
	GLint textureSampler2DLocation;
	GLint colorLocation;
	GLint blendFactorLocation;

public:
	static Renderer& get();

	void draw(GLfloat posCenterX, GLfloat posCenterY, GLfloat width, GLfloat height, GLfloat rotateAngle, const std::string& textureName2D, glm::vec3 color);

	inline const std::string& getVertexShaderPath() const { return this->VERTEX_SHADER_PATH; }
	inline const std::string& getFragmentShaderPath() const { return this->FRAGMENT_SHADER_PATH; }
};