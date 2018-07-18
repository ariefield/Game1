#include "MainGame.h"
#include "StaticMethods.h"

#include <iostream>
#include <fstream>
#include <string>

//Game related consts
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

//Define shader file paths
const char *VERTEX_PATH = "VertexShader.glsl";
const char *FRAGMENT_PATH = "FragmentShader.glsl";

//Define vertices of 2 triangles
const float vertices1[] = {
	-0.5f, -0.5f, 0.0f,		//bottom left	
	0.0f, 0.5f, 0.0f,		//top		
	0.5f, -0.5f, 0.0f,		//bottom right			
};

MainGame::MainGame()
{
	window = nullptr;
	screenWidth = 1024;
	screenHeight = 768;
	gameState = GameState::PLAY;
	frameStart = 0;
	frameTime = 0;

	//Move elsewhere
	vertexShader = 0;
	fragmentShader = 0;
	shaderProgram = 0;
	for (int i = 0; i < sizeof(VAOs); i++)
	{
		VAOs[i] = 0;
	}
	for (int i = 0; i < sizeof(VBOs); i++)
	{
		VBOs[i] = 0;
	}

	//Testing
	x1 = -0;
	y1 = 0.5;
	colorValue = 0;
}

MainGame::~MainGame()
{
}


void MainGame::run()
{
	initSystems();

	gameLoop();
}

void MainGame::initSystems()
{
	//Load shaders into memory
	std::string vertShaderStr = StaticMethods::readFile(VERTEX_PATH);
	std::string fragShaderStr = StaticMethods::readFile(FRAGMENT_PATH);
	const char *vertexShaderSource = vertShaderStr.c_str();
	const char *fragmentShaderSource = fragShaderStr.c_str();

	//Initialize SDL/glew/GL
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Game_Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		StaticMethods::fatalError("SDL window could not be created");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//int SDL_GL_SetSwapInterval(int interval);

	glewInit();

	//Clear screen to black
	glClearDepth(1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Create and bind VAO1
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// Load vertex data into buffer
	glGenBuffers(1, VBOs);							//Assign value to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);		//Set VBO as the buffer to work with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	//Tell the vertex shader how to interpret buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Set up vertex shader and compile
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Check if shader compiled properly
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX:COMPILATION_FAILED\n%s", infoLog);
	}

	//Set up fragment shader and compile
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Check if shader compiled properly
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT:COMPILATION_FAILED\n%s", infoLog);
	}

	//Create shader program 1
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::PROGRAM::COMPILATION_FAILED\n%s", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void MainGame::gameLoop()
{
	while (gameState != GameState::EXIT)
	{
		//TODO: decouple display framerate and simulation framerate -----------------------------------------------
		frameStart = SDL_GetTicks();

		processInput();
		update();
		render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < FRAME_DELAY)
		{
			SDL_Delay(FRAME_DELAY - frameTime);
		}
	}

	// de-allocate 
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
}

void MainGame::processInput()
{
	//Grabs user input 
	SDL_Event evnt;

	//Determine type of input
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			x1 = evnt.motion.x * 2 / screenWidth - 1;
			y1 = evnt.motion.y * 2 / screenHeight - 1;
		}
	}
}

void MainGame::update()
{
	colorValue += 0.02;
	float greenValue = (sin(colorValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}

void MainGame::render()
{
	//GL calls
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	glBindVertexArray(VAOs[0]);	
	glDrawArrays(GL_TRIANGLES, 0, 3);


	//Buffer next frame
	SDL_GL_SwapWindow(window);
}
