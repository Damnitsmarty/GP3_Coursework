#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include <glm\gtx\transform.hpp>


class Shader
{
public:
	Shader() {}
	Shader(const std::string& filename);

	void Bind(); //Set gpu to use our shaders
	//void Update(const glm::mat4& viewProjection, const glm::mat4& model);
	void Update(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);

	std::string Shader::LoadShader(const std::string& fileName);
	void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint Shader::CreateShader(const std::string& text, unsigned int type);
    ~Shader();


protected:
private:
	static const unsigned int NUM_SHADERS = 2; // number of shaders

	enum
	{
		U_MODEL,
		U_VIEW,
		U_PROJECTION,

		NUM_UNIFORMS
	};

	GLuint program; // Track the shader program
	GLuint shaders[NUM_SHADERS]; //array of shaders
	GLuint uniforms[NUM_UNIFORMS]; //no of uniform variables
};
