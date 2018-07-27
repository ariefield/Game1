#include "MainGame.h"

//Define shader file paths
const char *VERTEX_PATH = "VertexShader.vs";
const char *FRAGMENT_PATH = "FragmentShader.fs";

//Define vertices
const float vertices[] = {
	// positions        // colors
	0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
};

const float texCoords[] = {
	0.0f, 0.0f,  // lower-left corner  
	1.0f, 0.0f,  // lower-right corner
	0.5f, 1.0f   // top-center corner
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
	offsetX, offsetY = 0;
	//Move these elsewhere later
	up = false;
	down = false;
	left = false;
	right = false;
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
	SDL_GL_SetSwapInterval(1);

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

	//Generate texture ID, bind, and set wrapping/pixel picking options
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load image data
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container-texture.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//Attach image, generate mipmaps, and free image data
	stbi_image_free(data);
}

void MainGame::gameLoop()
{
	//TODO: De-couple simulation/display framerate -------------------
	//Set up display-related values
	const double DISPLAY_FPS = 60;
	const double MAX_DISPLAY_TIME = 1000.0 / DISPLAY_FPS;
	double elapsedFrameTime = 0;
	Uint64 frameStartCycles = SDL_GetTicks();
	double time = 0;

	printf("%f\n", MAX_DISPLAY_TIME);


	while (gameState != GameState::EXIT)
	{
		frameStartCycles = SDL_GetPerformanceCounter();

		processInput();
		update(time);

		elapsedFrameTime = 1000.0 * (double)(SDL_GetPerformanceCounter() - frameStartCycles) / SDL_GetPerformanceFrequency();

		if (elapsedFrameTime < MAX_DISPLAY_TIME)
		{
			while (elapsedFrameTime < MAX_DISPLAY_TIME)
			{
				elapsedFrameTime = 1000.0 * (double)(SDL_GetPerformanceCounter() - frameStartCycles) / SDL_GetPerformanceFrequency();
			}
		}
		else
		{
			printf("Game took %d milliseconds too long to render\n", elapsedFrameTime - MAX_DISPLAY_TIME );
		}

		render();

		time += elapsedFrameTime;
		printf("Elapsed Time:%f\n", elapsedFrameTime);
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
				break;

			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
					case SDLK_UP:
						up = true;
						break;
					case SDLK_DOWN:
						down = true;
						break;
					case SDLK_LEFT:
						left = true;
						break;
					case SDLK_RIGHT:
						right = true;
						break;
				} break;
					

			case SDL_KEYUP:
				switch (evnt.key.keysym.sym)
				{
					case SDLK_UP:
						up = false;
						break;
					case SDLK_DOWN:
						down = false;
						break;
					case SDLK_LEFT:
						left = false;
						break;
					case SDLK_RIGHT:
						right = false;
						break;
				} break;
		}
	}
}

void MainGame::update(double time)
{
	/*if (up)
	{
		offsetY += 0.025;
	}
	if (down)
	{
		offsetY -= 0.025;
	}
	if (left)
	{
		offsetX -= 0.025;
	}
	if (right)
	{
		offsetX += 0.025;
	}*/

	offsetX = 0.5*sin(0.001*time);

	shader->setFloat("offsetX", offsetX);
	shader->setFloat("offsetY", offsetY);
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
