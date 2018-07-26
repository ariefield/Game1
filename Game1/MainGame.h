#pragma once
#ifndef MAINGAME_H
#define MAINGAME_H

#include <SDL\SDL.h>
#include <GL\glew.h>

#include "stb_image.h"

#include "Shader.h"
#include "StaticMethods.h"

#include <iostream>
#include <fstream>
#include <string>

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();
	void run();

private:
	//SDL
	SDL_Window * window;
	int screenWidth;
	int screenHeight;
	GameState gameState;

	//OpenGL
	Shader *shader;
	GLuint VAOs[1];
	GLuint VBOs[1];

	//Testing
	float x1;
	float y1;
	float offsetX;
	float offsetY;
	//Move these elsewhere later
	bool up;
	bool down;
	bool left; 
	bool right;

	//Methods
	void initSystems();
	void gameLoop();
	void processInput();
	void update(double time);
	void render();
};

#endif
