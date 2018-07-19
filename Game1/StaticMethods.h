#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <string>

class StaticMethods
{
public:
	StaticMethods();
	~StaticMethods();

	static void fatalError(std::string errorString);
	static void checkProgramCompiled(unsigned int shader);
	static void checkShaderCompiled(unsigned int shader);
	static std::string readFile(const char *filePath);
};

