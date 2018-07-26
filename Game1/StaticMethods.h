#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class StaticMethods
{
public:
	StaticMethods();
	~StaticMethods();

	static void fatalError(std::string errorString);
	static std::string readFile(const char *filePath);
	static std::string readFileDeprecated(const char *filePath);
};

