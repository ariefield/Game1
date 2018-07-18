#pragma once
#include <SDL\SDL.h>
#include <iostream>
#include <fstream>
#include <string>

class StaticMethods
{
public:
	StaticMethods();
	~StaticMethods();

	static void fatalError(std::string errorString);
	static std::string readFile(const char *filePath);
};

