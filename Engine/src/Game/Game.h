#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"

class Game
{
public:
    Game();
    ~Game();
	
    void Initialize();
    void Run();
    void ProcessInput();
	void LoadLevel();
    void Setup();
    void Update();
	void FixedUpdate();
    void Render();
    void Destroy();

    void RenderTexture(const char* imagePath, int x, int y, int width, int height);


public:
    static int winWidth, winHeight;
    static int mapWidth, mapHeight;
	double GetDeltaTime()  { return m_deltaTime; }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
	SDL_Rect m_camera;
	
    std::unique_ptr<Registry> m_registry;
    std::unique_ptr<AssetManager> m_assetManager;
    std::unique_ptr<EventManager> m_eventManager;
    
    bool m_isRunning, m_isDebug;

    int m_milliSecLastFrame;
	double m_deltaTime, m_fixedDeltaTime;
};

#endif