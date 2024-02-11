#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "../AssetManager/AssetManager.h"
#include "../ECS/ECS.h"

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
	double GetDeltaTime()  { return m_deltaTime; }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    std::unique_ptr<Registry> m_registry;
    std::unique_ptr<AssetManager> m_assetManager;
    
    bool m_isRunning, m_isDebug;
    int m_winWidth, m_winHeight;

    int m_milliSecLastFrame;
	double m_deltaTime, m_fixedDeltaTime;
};

#endif