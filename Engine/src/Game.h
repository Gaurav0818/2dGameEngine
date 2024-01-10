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

    void DisplayImage(const char* imagePath, int x, int y, int width, int height);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool isRunning;
    int winWidth;
    int winHeight;
};

#endif