#include "Game.h"

#include <fstream>
#include<glm/glm.hpp>
#include<SDL_image.h>

#include "../Components/BoxColliderComponent.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"

#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/BoxColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardInputSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderBoxCollisionSystem.h"
#include "../Systems/RenderSystem.h"

struct AnimationComponent;
Game::Game(): m_window(nullptr), m_renderer(nullptr), m_winWidth(500), m_winHeight(500), m_milliSecLastFrame(0)
{
	m_isRunning = true;
	m_registry = std::make_unique<Registry>();
	m_assetManager = std::make_unique<AssetManager>();
	m_eventManager = std::make_unique<EventManager>();
	
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
		FixedUpdate();
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
			if(event.key.keysym.sym == SDLK_d)
				m_isDebug = !m_isDebug;

			m_eventManager->EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
			
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
	m_registry->AddSystem<AnimationSystem>();
	m_registry->AddSystem<BoxColliderSystem>();
	m_registry->AddSystem<RenderBoxCollisionSystem>();
	m_registry->AddSystem<DamageSystem>();
	m_registry->AddSystem<KeyboardInputSystem>();

	m_assetManager->AddTexture( m_renderer, "tank-image", "./assets/images/tank-panther-right.png");
	m_assetManager->AddTexture( m_renderer, "tileMap-image", "./assets/tilemaps/jungle.png");
	m_assetManager->AddTexture( m_renderer, "chopper-image", "./assets/images/chopper.png");
	m_assetManager->AddTexture( m_renderer, "radar-image", "./assets/images/radar.png");

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
			tile.AddComponent<SpriteComponent>("tileMap-image", tileSize, tileSize, 0, sourceRectX, sourceRectY);
		}
	}
	mapFile.close();
	
	// Create some entities
	Entity chopper = m_registry->CreateEntity();
	
	// Add a Component to the entity
	chopper.AddComponent<TransformComponent>(glm::vec2(100, 100), glm::vec2(3, 3), 0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(10, 0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	chopper.AddComponent<AnimationComponent>(2, 15, true);
	chopper.AddComponent<BoxColliderComponent>(32, 32);

	Entity radar = m_registry->CreateEntity();

	radar.AddComponent<TransformComponent>(glm::vec2(250, 75), glm::vec2(2,2), 0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2);
	radar.AddComponent<AnimationComponent>(8, 8, true);
	radar.AddComponent<BoxColliderComponent>(64, 64);
	
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

	// Reset all event handlers for the current frame
	m_eventManager->Reset();
	
	//Perform the subscription to the events
	m_registry->GetSystem<DamageSystem>().SubscribeToEvents(m_eventManager);
	m_registry->GetSystem<KeyboardInputSystem>().SubscribeToEvents(m_eventManager);
	
	// Update the Registry
	m_registry->Update();

	// Invoke all the Systems that needs to update
	m_registry->GetSystem<MovementSystem>().Update(m_deltaTime);
	m_registry->GetSystem<AnimationSystem>().Update();

}

void Game::FixedUpdate()
{
	m_fixedDeltaTime += m_deltaTime;
	
	if(m_fixedDeltaTime < 1.0f/30)
		return;

	m_fixedDeltaTime = 0.0f;
	
	m_registry->GetSystem<BoxColliderSystem>().Update(m_eventManager);
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
	
	if(m_isDebug)
		m_registry->GetSystem<RenderBoxCollisionSystem>().Update(m_renderer);

	SDL_RenderPresent(m_renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}