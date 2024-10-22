#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../WindowManager/WindowManager.h"

Renderer::Renderer()
	: VERTEX_SHADER_PATH("shaders/vertexShader.txt"), FRAGMENT_SHADER_PATH("shaders/fragmentShader.txt")
	, vertexShaderSource(readShader(VERTEX_SHADER_PATH)), fragmentShaderSource(readShader(FRAGMENT_SHADER_PATH))
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexShaderSource = this->vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	const char* fragmentShaderSource = this->fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, vertexShader);
	glAttachShader(this->shaderProgram, fragmentShader);
	glLinkProgram(this->shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(this->shaderProgram);

	this->projectionMatrixLocation = glGetUniformLocation(this->shaderProgram, "projectionMatrix");
	this->textureSampler2DLocation = glGetUniformLocation(this->shaderProgram, "textureSampler2D");
	this->colorLocation = glGetUniformLocation(this->shaderProgram, "color");
	this->blendFactorLocation = glGetUniformLocation(this->shaderProgram, "blendFactor");

	std::vector<GLfloat> coordinates = {
		// positions            // texture coords
		-0.5f, -0.5f,			0.0f, 0.0f,
		0.5f, -0.5f,			1.0f, 0.0f,
		0.5f, 0.5f,				1.0f, 1.0f,
		-0.5f, 0.5f,			0.0f, 1.0f
	};

	std::vector<GLuint> indices = {
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

Renderer::~Renderer()
{
	glDeleteBuffers(1, &this->EBO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteVertexArrays(1, &this->VAO);
}

Renderer& Renderer::get()
{
	static Renderer instance;
	return instance;
}

void Renderer::draw(GLfloat posCenterX, GLfloat posCenterY, GLfloat width, GLfloat height, GLfloat rotateAngle, const std::string& textureName2D, glm::vec3 color)
{

}

std::string Renderer::readShader(const std::string& shaderPath)
{
	std::ifstream inputFileStream(shaderPath);
	if (!inputFileStream.is_open())
	{
		std::cout << "Error: Could not open shader file from: " << shaderPath << std::endl;
	}

	std::stringstream stringStream;
	stringStream << inputFileStream.rdbuf();

	std::cout << "Shader file from " << shaderPath << " content: " << stringStream.str() << std::endl;

	return stringStream.str();
}