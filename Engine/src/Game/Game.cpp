#include "Game.h"

#include <fstream>
#include<glm/glm.hpp>
#include<SDL_image.h>

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"

#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"

Game::Game(): m_window(nullptr), m_renderer(nullptr), m_winWidth(500), m_winHeight(500), m_milliSecLastFrame(0)
{
	m_isRunning = true;
	m_registry = std::make_unique<Registry>();
	m_assetManager = std::make_unique<AssetManager>();
	
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

	m_winWidth = 1000;// displayMode.w;
	m_winHeight = 1000;// displayMode.h;

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
void Game::LoadLevel()
{
	// Add a System to the Registry
	m_registry->AddSystem<MovementSystem>();
	m_registry->AddSystem<RenderSystem>();

	m_assetManager->AddTexture( m_renderer, "tank-image", "./assets/images/tank-panther-right.png");
	m_assetManager->AddTexture( m_renderer, "tileMap-image", "./assets/tilemaps/jungle.png");

	// Load the tileMap
	int tileSize = 32;
	double tileScale = 3.0;
	int mapNumCols = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for(int y = 0; y <mapNumRows; y++)
	{
		for(int x = 0; x < mapNumCols; x++)
		{
			char ch;
			mapFile.get(ch);
			int sourceRectY = atoi(&ch) * tileSize;
			mapFile.get(ch);
			int sourceRectX = atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = m_registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * tileSize * tileScale, y * tileSize * tileScale), glm::vec2(tileScale, tileScale), 0);
			tile.AddComponent<SpriteComponent>("tileMap-image", tileSize, tileSize, sourceRectX, sourceRectY);
		}
	}
	mapFile.close();
	
	// Create some entities
	Entity tank = m_registry->CreateEntity();
	
	// Add a Component to the entity
	tank.AddComponent<TransformComponent>(glm::vec2(100, 100), glm::vec2(1, 1), 0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(10, 0));
	tank.AddComponent<SpriteComponent>("tank-image",32,32);
}

/**
 * @brief Initialize Stuff
 */
void Game::Setup()
{
	LoadLevel();
}

/**
 * @brief
 * Update Stuff
 */
void Game::Update()
{
	m_deltaTime = (SDL_GetTicks() - m_milliSecLastFrame) / 1000.0f;
	// Store current time as Previous Frame Time
	m_milliSecLastFrame = SDL_GetTicks();
	
	// Update the Registry
	m_registry->Update();

	// Invoke all the Systems that needs to update
	m_registry->GetSystem<MovementSystem>().Update(m_deltaTime);
}

/**
 * @brief
 * Render Stuff
 */
void Game::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 120, 150, 255);
	SDL_RenderClear(m_renderer);

	// Invoke all the Systems that needs to Render
	m_registry->GetSystem<RenderSystem>().Update(m_renderer, m_assetManager);

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