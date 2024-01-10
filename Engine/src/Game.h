#ifndef GAME_H
#define GAME_H

#include <SDL.h>

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

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool isRunning;
    int winWidth;
    int winHeight;
};

#endif