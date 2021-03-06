#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <SDL\SDL.h>
#include <GL\glew.h>

#include "StaticMethods.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// -- Variables --
	//Program ID
	GLuint ID;

	// -- Methods --
	//Constructor+Destructor
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();
	//Activate the shader
	void use();
	//Uniform-setting functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

private:
	//Helper functions for compilation
	void checkProgramCompiled(unsigned int shader);
	void checkShaderCompiled(unsigned int shader);
};

#endif
