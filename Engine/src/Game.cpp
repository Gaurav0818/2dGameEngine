#include "Game.h"
#include <iostream>

#include<glm/glm.hpp>
#include<SDL_image.h>

#include "ECS/ECS.h"
#include "Logger.h"

Game::Game()
{
	isRunning = true;
	Logger::Info("Game Constructor");
}

Game::~Game()
{
	Logger::Info("Game Destructor");
}

/// <summary>
/// Initialize window and renderer
/// </summary>

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Error("Error Initializing SDL.");
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
		Logger::Error("Error Creating SDL Window.");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		Logger::Error("Error Creating SDL Renderer.");
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

/// <summary>
/// Initialize Stuff
/// </summary>
void Game::Setup()
{
	// TODO:
	// Enitiy tank = registry.CreateEnitity();
	// tank.AddComponent<TransformComponent>();
	// tank.AddComponent<SpriteComponent>();
	// tank.AddComponent<BoxColliderComponent>();

}


/// <summary>
/// Update Stuff
/// </summary>
void Game::Update()
{
	DeltaTime = (SDL_GetTicks() - millisecLastFrame) / 1000.0f;
	
	millisecLastFrame = SDL_GetTicks();

	//TODO:
	// MovementSystem.Update();
	// CollisionSystem.Update();
	// ....
}



/// <summary>
/// Render Stuff
/// </summary>
void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 0, 120, 150, 255);
	SDL_RenderClear(renderer);

	//TODO: Render Game Objects...

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