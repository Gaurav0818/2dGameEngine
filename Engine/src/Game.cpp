#include "Game.h"
#include <iostream>

#include<glm/glm.hpp>
#include<SDL_image.h>

Game::Game()
{
	std::cout<<"Game Constructor" << std::endl;	
}

Game::~Game()
{
	std::cout<<"Game Destructor" << std::endl;
}

/// <summary>
/// Initialize window and renderer
/// </summary>

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << " Error initializing SDL." << std::endl;
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	winWidth = 1024;// displayMode.w;
	winHeight = 1024;// displayMode.h;

	window = SDL_CreateWindow(
				NULL, 
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
				winWidth, winHeight,
				SDL_WINDOW_BORDERLESS);

	if (!window)
	{
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}
}

/// <summary>
/// Game Loop
/// </summary>
void Game::Run()
{
	Setup();
	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

/// <summary>
/// Takes Care of Input
/// </summary>
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;
			break;
		default:
			break;
		}
	}
}

glm::vec2 position;
glm::vec2 velocity;

/// <summary>
/// Initialize Stuff
/// </summary>
void Game::Setup()
{
	isRunning = true;

	position = glm::vec2(0.0f, 0.0f);
	velocity = glm::vec2(100.0f, 10.0f);
}


/// <summary>
/// Update Stuff
/// </summary>
void Game::Update()
{
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), millisecLastFrame + MILLISEC_PER_FRAME)) {
	//	std::cout << "Waiting for next frame" << std::endl;
	//}

	int timeToWait = MILLISEC_PER_FRAME - (SDL_GetTicks() - millisecLastFrame);
	if(timeToWait> 0 && timeToWait < MILLISEC_PER_FRAME )
		SDL_Delay(timeToWait);
	
	DeltaTime = (SDL_GetTicks() - millisecLastFrame) / 1000.0f;
	
	millisecLastFrame = SDL_GetTicks();



	

	position.x += velocity.x *DeltaTime;
	position.y += velocity.y *DeltaTime;

	if (position.x > winWidth)
		position.x = 0;
	
	if (position.y > winHeight)
		position.y = 0;
}



/// <summary>
/// Render Stuff
/// </summary>
void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 0, 120, 150, 255);
	SDL_RenderClear(renderer);

	RenderTexture("./assets/images/tree.png", position.x, position.y, 100, 100);

	SDL_RenderPresent(renderer);
}


void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::RenderTexture(const char* imagePath, int x, int y, int width, int height)
{
	// Load a Collection of Pixels into a Surface
	SDL_Surface* surface = IMG_Load(imagePath);
	// Create a Texture from Collection of Pixels
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	// Free the buffer of Pixels 
	SDL_FreeSurface(surface);

	// Define a Rectangle to Render the Texture
	SDL_Rect destRect = { x, y, width, height };
	// Render the Texture to the Screen using the Renderer and The Rectangle 
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
	// Free the Texture 
	SDL_DestroyTexture(texture);
}