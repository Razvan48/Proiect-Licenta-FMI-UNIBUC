#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../WindowManager/WindowManager.h"
#include "../AssetManager/AssetManager.h"

Renderer::Renderer()
	: VERTEX_SHADER_PATH("shaders/vertexShader.txt"), FRAGMENT_SHADER_PATH("shaders/fragmentShader.txt")
	, vertexShaderSource(this->readShader(VERTEX_SHADER_PATH)), fragmentShaderSource(this->readShader(FRAGMENT_SHADER_PATH))
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

	this->transformationMatrixLocation = glGetUniformLocation(this->shaderProgram, "transformationMatrix");
	this->textureSampler2DLocation = glGetUniformLocation(this->shaderProgram, "textureSampler2D");
	// glActiveTexture(GL_TEXTURE0); // este scrisa in metoda draw()
	glUniform1i(this->textureSampler2DLocation, 0);
	this->colorLocation = glGetUniformLocation(this->shaderProgram, "color");
	this->blendFactorLocation = glGetUniformLocation(this->shaderProgram, "blendFactor");
	this->isDrawingTextLocation = glGetUniformLocation(this->shaderProgram, "isDrawingText");

	this->coordinates = {
		// positions            // texture coords
		-0.5f, -0.5f,			0.0f, 0.0f,
		0.5f, -0.5f,			1.0f, 0.0f,
		0.5f, 0.5f,				1.0f, 1.0f,
		-0.5f, 0.5f,			0.0f, 1.0f
	};

	this->indices = {
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->coordinates.size() * sizeof(GLfloat), this->coordinates.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Pentru desenare de text, freetype (pare sa mearga si fara)
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer()
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &this->EBO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteVertexArrays(1, &this->VAO);

	glUseProgram(0);
	glDeleteProgram(this->shaderProgram);
}

Renderer& Renderer::get()
{
	static Renderer instance;
	return instance;
}

void Renderer::draw(GLfloat posCenterX, GLfloat posCenterY, GLfloat width, GLfloat height, GLfloat rotateAngle, const std::string& textureName2D, glm::vec3 color, float blendFactor, bool isDrawingText)
{
	glActiveTexture(GL_TEXTURE0); // Din cauza ca shader-ul contine o singura textura, am putea sa mutam aceasta linie in constructor-ul clasei
	glBindTexture(GL_TEXTURE_2D, AssetManager::get().getTexture(textureName2D));

	// glUseProgram(this->shaderProgram); // Nu schimbam shader-ul, deoarece avem doar unul

	glm::mat4 transformationMatrix = 
		glm::ortho(0.0f, (GLfloat)WindowManager::get().getWindowWidth(), 0.0f, (GLfloat)WindowManager::get().getWindowHeight())
		* glm::translate(glm::mat4(1.0f), glm::vec3(posCenterX, posCenterY, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 1.0f));

	glUniformMatrix4fv(this->transformationMatrixLocation, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

	glUniform4f(this->colorLocation, color.x, color.y, color.z, 1.0f);
	glUniform1f(this->blendFactorLocation, blendFactor);
	glUniform1i(this->isDrawingTextLocation, isDrawingText);

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::draw(GLfloat posCenterX, GLfloat posCenterY, GLfloat width, GLfloat height, GLfloat rotateAngle, GLuint& texture, glm::vec3 color, float blendFactor, bool isDrawingText)
{
	glActiveTexture(GL_TEXTURE0); // Din cauza ca shader-ul contine o singura textura, am putea sa mutam aceasta linie in constructor-ul clasei
	glBindTexture(GL_TEXTURE_2D, texture);

	// glUseProgram(this->shaderProgram); // Nu schimbam shader-ul, deoarece avem doar unul

	glm::mat4 transformationMatrix =
		glm::ortho(0.0f, (GLfloat)WindowManager::get().getWindowWidth(), 0.0f, (GLfloat)WindowManager::get().getWindowHeight())
		* glm::translate(glm::mat4(1.0f), glm::vec3(posCenterX, posCenterY, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 1.0f));

	glUniformMatrix4fv(this->transformationMatrixLocation, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

	glUniform4f(this->colorLocation, color.x, color.y, color.z, 1.0f);
	glUniform1f(this->blendFactorLocation, blendFactor);
	glUniform1i(this->isDrawingTextLocation, isDrawingText);

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::drawText(GLfloat posCenterX, GLfloat posCenterY, GLfloat width, const std::string& fontName, const std::string& text, glm::vec3 color, float blendFactor)
{
	std::vector<AssetManager::Character>& font = AssetManager::get().getFont(fontName);

	GLfloat actualWidth = 0.0f;
	for (int i = 0; i < text.size(); ++i)
	{
		actualWidth += (font[text[i]].advance >> 6); // .advance masoara o unitate pentru fiecare 1/64 pixel, (64 de unitati pentru un pixel) => impartim prin 64
	}

	GLfloat scale = width / actualWidth;

	GLfloat currentPosX = posCenterX - width / 2.0f;
	GLfloat currentPosY = posCenterY;

	for (int i = 0; i < text.size(); ++i)
	{
		AssetManager::Character& character = font[text[i]];

		GLfloat posCenterCharacterX = currentPosX + character.bearing.x * scale;
		GLfloat posCenterCharacterY = currentPosY - (character.size.y - character.bearing.y) * scale;

		GLfloat characterWidth = character.size.x * scale;
		GLfloat characterHeight = character.size.y * scale;

		this->draw(posCenterCharacterX + characterWidth / 2.0f,
			posCenterCharacterY + characterHeight / 2.0f,
			characterWidth, characterHeight, 0.0f,
			font[text[i]].texture, color, blendFactor, true);

		currentPosX += (character.advance >> 6) * scale;
	}
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

	// std::cout << "Shader file from " << shaderPath << " content: " << stringStream.str() << std::endl;

	return stringStream.str();
}