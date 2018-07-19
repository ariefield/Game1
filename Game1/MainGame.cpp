#include "MainGame.h"

//Define shader file paths
const char *VERTEX_PATH = "VertexShader.vs";
const char *FRAGMENT_PATH = "FragmentShader.fs";

//Define vertices
const float vertices[] = {
	// positions        // colors
	0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
};

MainGame::MainGame()
{
	//SDL
	window = nullptr;
	screenWidth = 1024;
	screenHeight = 768;
	gameState = GameState::PLAY;

	//OpenGL
	shader = nullptr;
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
	// -- Initialize SDL related objects
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Game1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		StaticMethods::fatalError("SDL window could not be created");
	}
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// -- Initialize OpenGL related objects
	glewInit();
	shader = new Shader(VERTEX_PATH, FRAGMENT_PATH);

	//Create and bind VAO1
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// Load vertex data into buffer
	glGenBuffers(1, VBOs);						//Assign value to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);		//Set VBO as the buffer to work with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Tell the vertex shader how to interpret buffer position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Tell the vertex shader how to interpret buffer color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void MainGame::gameLoop()
{
	//TODO: De-couple simulation/display framerate -------------------
	//Set up display-related values
	const int DISPLAY_FPS = 60;
	const int MAX_DISPLAY_TIME = 1000 / DISPLAY_FPS;
	Uint32 frameStartTime = SDL_GetTicks();
	int currentFrameTime = 0;

	while (gameState != GameState::EXIT)
	{
		frameStartTime = SDL_GetTicks();

		processInput();
		update();
		render();

		currentFrameTime = SDL_GetTicks() - frameStartTime;

		if (currentFrameTime < MAX_DISPLAY_TIME)
		{
			SDL_Delay(MAX_DISPLAY_TIME - currentFrameTime);
			while (currentFrameTime < MAX_DISPLAY_TIME)
			{
				currentFrameTime = SDL_GetTicks() - frameStartTime;
			}
		}
		else
		{
			printf("Game took %d milliseconds too long to render\n", currentFrameTime - MAX_DISPLAY_TIME );
		}
	}

	// De-allocate after game is exited
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

}

void MainGame::render()
{
	//Clear screen
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw triangle
	shader->use();
	glBindVertexArray(VAOs[0]);	
	glDrawArrays(GL_TRIANGLES, 0, 3);


	//Buffer next frame
	SDL_GL_SwapWindow(window);
}
