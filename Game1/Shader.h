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
	//Program ID
	GLuint ID;

	//Methods
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();
	//Use/activate the shader
	void use();
	//Utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif
