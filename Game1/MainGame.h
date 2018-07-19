#pragma once

#include <SDL\SDL.h>
#include <GL\glew.h>

enum class GameState {PLAY, EXIT};

class MainGame
{
	public:
		MainGame();
		~MainGame();

		void run();

 
	private:
		//Functions
		void initSystems();
		void gameLoop();
		void processInput();
		void update();
		void render();

		//Variables
		SDL_Window * window;
		int screenWidth;
		int screenHeight;
		GameState gameState;

		//Vars to be moved elsewhere
		GLuint VAOs[2];
		GLuint VBOs[2];
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint shaderProgram;
		//Vars for testing
		float x1;
		float y1;
		float colorValue;

};

