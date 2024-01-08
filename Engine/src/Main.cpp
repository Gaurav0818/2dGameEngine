#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>



int main(int argc, char* argv[]) {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    glm::vec2 vector = glm::vec2(2.0, 1.0);
    SDL_Init(SDL_INIT_EVERYTHING);
    std::cout << "Hello, world!" << std::endl;
    return 0;
}