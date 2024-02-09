#include "Game.h"
#include <iostream>

#include<glm/glm.hpp>
#include<SDL_image.h>

#include "ECS/ECS.h"
#include "Logger.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"

Game::Game(): m_window(nullptr), m_renderer(nullptr), m_winWidth(500), m_winHeight(500), m_milliSecLastFrame(0), m_deltaTime(0)
{
	m_isRunning = true;
	m_registry = std::make_unique<Registry>();
	
	Logger::Info("Game Constructor");
}

Game::~Game()
{
	Logger::Info("Game Destructor");
}


/**
 * @brief
 * Initialize window and renderer
 */
void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Error("Error Initializing SDL.");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	m_winWidth = 1024;// displayMode.w;
	m_winHeight = 1024;// displayMode.h;

	m_window = SDL_CreateWindow(
				nullptr, 
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
				m_winWidth, m_winHeight,
				SDL_WINDOW_BORDERLESS);

	if (!m_window)
	{
		Logger::Error("Error Creating SDL Window.");
		return;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, 0);

	if (!m_renderer)
	{
		Logger::Error("Error Creating SDL Renderer.");
		return;
	}
}

/**
 * @brief
 * Game Loop
 */
void Game::Run()
{
	Setup();
	while (m_isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

/**
 * @brief 
 * Takes Care of Input
 */
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_isRunning = false;
			break;
		default:
			break;
		}
	}
}

/**
 * @brief Initialize Stuff
 */
void Game::Setup()
{
	// Create some entities
	Entity tank = m_registry->CreateEntity();
	
	// Add a Component to the entity
	m_registry->AddComponent<TransformComponent>(tank, glm::vec2(100, 100), glm::vec2(1, 1), 0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(-10, 0));
	tank.RemoveComponent<TransformComponent>();
}


/**
 * @brief
 * Update Stuff
 */
void Game::Update()
{
	m_deltaTime = (SDL_GetTicks() - m_milliSecLastFrame) / 1000.0f;
	
	m_milliSecLastFrame = SDL_GetTicks();

	//TODO:
	// MovementSystem.Update();
	// CollisionSystem.Update();
	// ....
}



/**
 * @brief
 * Render Stuff
 */
void Game::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 120, 150, 255);
	SDL_RenderClear(m_renderer);

	//TODO: Render Game Objects...

	SDL_RenderPresent(m_renderer);
}


void Game::Destroy()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Game::RenderTexture(const char* imagePath, int x, int y, int width, int height)
{
	// Load a Collection of Pixels into a Surface
	SDL_Surface* surface = IMG_Load(imagePath);
	// Create a Texture from Collection of Pixels
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	// Free the buffer of Pixels 
	SDL_FreeSurface(surface);

	// Define a Rectangle to Render the Texture
	SDL_Rect destRect = { x, y, width, height };
	// Render the Texture to the Screen using the Renderer and The Rectangle 
	SDL_RenderCopy(m_renderer, texture, NULL, &destRect);
	// Free the Texture 
	SDL_DestroyTexture(texture);
}