#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	GLint transformationMatrixLocation;
	GLint textureSampler2DLocation;
	GLint colorLocation;
	GLint blendFactorLocation;
	GLint isDrawingTextLocation;

	std::vector<GLfloat> coordinates;
	std::vector<GLuint> indices;

public:
	static Renderer& get();

	void draw(GLfloat posCenterX, GLfloat posCenterY, GLfloat width, GLfloat height, GLfloat rotateAngle, const std::string& textureName2D, glm::vec3 color, float blendFactor, bool isDrawingText);
	void draw(GLfloat posCenterX, GLfloat posCenterY, GLfloat width, GLfloat height, GLfloat rotateAngle, GLuint& texture, glm::vec3 color, float blendFactor, bool isDrawingText);
	void drawText(GLfloat posCenterX, GLfloat posCenterY, GLfloat width, GLfloat rotateAngle, const std::string& fontName, const std::string& text, glm::vec3 color, float blendFactor);

	inline const std::string& getVertexShaderPath() const { return this->VERTEX_SHADER_PATH; }
	inline const std::string& getFragmentShaderPath() const { return this->FRAGMENT_SHADER_PATH; }
};