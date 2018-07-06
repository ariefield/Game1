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
		void drawGame();

		//Variables
		SDL_Window * _window;
		int _screenWidth;
		int _screenHeight;
		GameState _gameState;
		//Vars to be moved elsewhere
		GLuint _VAOs[2];
		GLuint _VBOs[2];
		GLuint _EBO;
		GLuint _vertexShader;
		GLuint _fragmentShader1;
		GLuint _fragmentShader2;
		GLuint _shaderProgram1;
		GLuint _shaderProgram2;
		//Vars for testing
		float x1;
		float y1;

};

