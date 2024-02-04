#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "ECS/ECS.h"

class Game
{
public:
    Game();
    ~Game();

    void Initialize();
    void Run();
    void ProcessInput();
    void Setup();
    void Update();
    void Render();
    void Destroy();

    void RenderTexture(const char* imagePath, int x, int y, int width, int height);

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    std::unique_ptr<Registry> m_registry;
    
    bool m_isRunning;
    int m_winWidth, m_winHeight;

    int m_milliSecLastFrame;
    double m_deltaTime;
};

#endif