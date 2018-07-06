#include "MainGame.h"

#include <iostream>
#include <string>

//Constants
//Define vertex shader
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";

//Define fragment shader
const char *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColour;\n"
"void main()\n"
"{ FragColour = vec4(0.3f, 0.6f, 0.9f, 0.5f);}";

//Define fragment shader
const char *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColour;\n"
"void main()\n"
"{ FragColour = vec4(1.0f, 1.0f, 0.0f, 0.5f);}";

//Define vertices of 2 triangles
float vertices1[] = {
	-0.5f,  0.0f, 0.0f,		//bottom left	0
	-0.25f, 0.5f, 0.0f,		//top left		1
	0.0, 0.0f, 0.0f,		//mid			2
};

float vertices2[] = {
	0.0, 0.0f, 0.0f,		//mid			2
	0.25f, 0.5f, 0.0f,		//top right		3
	0.5f, 0.0f, 0.0f		//bottom right	4
};

unsigned int indices1[] = {  // note that we start from 0!
	0, 1, 2,	// first triangle
	2, 3, 4		// second triangle
};

void fatalError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
	_gameState = GameState::PLAY;
	//Move elsewhere
	_EBO = 0;
	_vertexShader = 0;
	_fragmentShader1 = 0;
	_fragmentShader2 = 0;
	_shaderProgram1 = 0;
	_shaderProgram2 = 0;
	for(int i=0;i<sizeof(_VAOs);i++)
		_VAOs[i] = 0;
	for (int i = 0; i<sizeof(_VBOs); i++)
		_VBOs[i] = 0;
	//Testing
	x1 = -0;
	y1 = 0.5;
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
	//Initialize SDL/glew/GL
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow("Game_Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr)
	{
		fatalError("SDL window could not be created");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glewInit();

	//Clear screen to black
	glClearDepth(1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Create and bind VAO1
	glGenVertexArrays(2, _VAOs);
	glBindVertexArray(_VAOs[0]);

	// Set up and load vertex data into buffer
	glGenBuffers(2, _VBOs);							//Assign value to VBO
	glBindBuffer(GL_ARRAY_BUFFER, _VBOs[0]);		//Set VBO as the buffer to work with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	////Set up and load element data into buffer
	//glGenBuffers(1, &_EBO1);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO1);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	//Tell the vertex shader how to interpret buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Create and bind VAO2
	glBindVertexArray(_VAOs[1]);

	// Set up and load vertex data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, _VBOs[1]);     //Set VBO as the buffer to work with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	////Set up and load element data into buffer
	//glGenBuffers(1, &_EBO1);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO1);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	//Tell the vertex shader how to interpret buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Set up vertex shader and compile
	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(_vertexShader);

	//Check if shader compiled properly
	int success;
	char infoLog[512];
	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(_vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX:COMPILATION_FAILED\n%s", infoLog);
	}

	//Set up fragment shader1 and compile
	_fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(_fragmentShader1);

	//Set up fragment shader2 and compile
	_fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(_fragmentShader2);

	//Check if shader compiled properly
	glGetShaderiv(_fragmentShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(_fragmentShader1, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT:COMPILATION_FAILED\n%s", infoLog);
	}

	//Create shader program 1
	_shaderProgram1 = glCreateProgram();
	glAttachShader(_shaderProgram1, _vertexShader);
	glAttachShader(_shaderProgram1, _fragmentShader1);
	glLinkProgram(_shaderProgram1);

	//Create shader program 2
	_shaderProgram2 = glCreateProgram();
	glAttachShader(_shaderProgram2, _vertexShader);
	glAttachShader(_shaderProgram2, _fragmentShader2);
	glLinkProgram(_shaderProgram2);

	glGetProgramiv(_shaderProgram1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_shaderProgram1, 512, NULL, infoLog);
		printf("ERROR::PROGRAM::COMPILATION_FAILED\n%s", infoLog);
	}

	glDeleteShader(_vertexShader);
	glDeleteShader(_fragmentShader1);
	glDeleteShader(_fragmentShader2);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
	// de-allocate 
	glDeleteVertexArrays(2, _VAOs);
	glDeleteBuffers(2, _VBOs);
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
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				x1 = evnt.motion.x * 2 / _screenWidth - 1;
				y1 = evnt.motion.y * 2 / _screenHeight - 1;
		}
	}
}

void MainGame::drawGame()
{
	//Draw first triangle
	glUseProgram(_shaderProgram1);
	glBindVertexArray(_VAOs[0]);	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glUseProgram(_shaderProgram2);
	glBindVertexArray(_VAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	SDL_GL_SwapWindow(_window);
}
