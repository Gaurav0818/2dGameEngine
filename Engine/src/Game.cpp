#include "Game.h"
#include <iostream>

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
	isRunning = true;

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

/// <summary>
/// Initialize Stuff
/// </summary>
void Game::Setup()
{
}

/// <summary>
/// Update Stuff
/// </summary>
void Game::Update()
{
	// Update stuff...
}



/// <summary>
/// Render Stuff
/// </summary>
void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 0, 120, 150, 255);
	SDL_RenderClear(renderer);

	RenderTexture("./assets/images/tree.png", 0, 0, 100, 100);

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
	SDL_Surface* surface = IMG_Load(imagePath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect destRect = { x, y, width, height };
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}