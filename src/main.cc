#include <SDL2/SDL.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <vector>
// #include <ostream>

// #include "PObject.hh"
#include "AliveEntity.hh"
#include "Player.hh"

#include "Debug.hh"
#include "Vector.hh"

int init_SDL(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height);
int update(SDL_Event* e, Vector* force, Vector* viewport);

int main(int argc, char** argv) 
{
    printf("%d %s", argc, *argv);

    debug.enabled = true;

    SDL_Window*   window        = nullptr;
    SDL_Renderer* renderer      = nullptr;
    const int     SCREEN_WIDTH  = 1280;
    const int     SCREEN_HEIGHT = 720;

    if (init_SDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT) != 0) return 1;

    Vector viewport = {0, 0};
    Vector force{0, 0};
    Player* player = new Player(256, renderer, &viewport, "MonkeyMagnus");

    // initierar enemies och lägger de i en array
    std::vector<AliveEntity*> AliveEntities;
    AliveEntities.reserve(20);
    AliveEntities.emplace_back(player);

    for (std::size_t i = 0; i < 12; i++)
    {
        AliveEntity enemy = AliveEntity(int(i), renderer, &viewport);
        Vector position = { double(i) * 50, double(i * i) * 10 };

        std::cout << fmt::format("x: {} y: {}\n", position.x, position.y);

        enemy.setWorldPosition(position);

        std::cout << fmt::format("x: {} y: {}\n", enemy.getWorldPosition().x, enemy.getWorldPosition().y);

        AliveEntities.emplace_back(&enemy);
    }


    // tjofs för deltatime å det
    const double MS_PER_UPDATE = 1.0 / 60.0; // 60Hz physics
    double       accumulator   = 0.0;

    Uint64 previous = SDL_GetPerformanceCounter();
    double freq     = double(SDL_GetPerformanceFrequency());

    bool running = true;
    int  frameId = 0;

    while (running) 
    {
        debug.setPrefix(fmt::format("[{:04}] ", frameId++));

        Uint64 current    = SDL_GetPerformanceCounter();
        double frameTime  = double(current - previous) / freq;
        previous          = current;
        accumulator      += frameTime;
        force             = {0, 0};
        SDL_Event e;

        int res = update(&e, &force, &viewport);
        if (res) running = false;

        while (accumulator >= MS_PER_UPDATE) 
        {
            player->applyForce(force);
            player->update(MS_PER_UPDATE);
            accumulator -= MS_PER_UPDATE;
        }

        bool FPV = true;
        if (FPV)
        {
            // Attaching viewport to player position
            viewport    = player->getWorldPosition();

            // Centering player to screen and taking dimensions
            // in to account
            viewport   += player->getDimensions() / 2;
            viewport.x -= int(double(SCREEN_WIDTH) / 2);
            viewport.y -= int(double(SCREEN_HEIGHT) / 2);
        }

        // std::cout << "[ viewport position ]\tx: " << viewport.x << "y: " << viewport.y << std::endl;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for (std::size_t i = 0; i < AliveEntities.size(); i++)
        {
            /// TODO: make standard implementation:
            AliveEntities[i]->draw();
        }

        SDL_RenderPresent(renderer);
    }

    delete player;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int update(SDL_Event* e, Vector* force, Vector* viewport)
{

    while (SDL_PollEvent(e)) 
    {
        if (e->type == SDL_QUIT)
            return 1;
    }

    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    const int standard_force = 7000; 
    if (keys[SDL_SCANCODE_H]) force->x -= standard_force;
    if (keys[SDL_SCANCODE_L]) force->x += standard_force;
    if (keys[SDL_SCANCODE_J]) force->y += standard_force;
    if (keys[SDL_SCANCODE_K]) force->y -= standard_force;
    if (keys[SDL_SCANCODE_Q]) return 1;

    const int viewport_speed = 4;
    if (keys[SDL_SCANCODE_W]) viewport->y -= viewport_speed;
    if (keys[SDL_SCANCODE_A]) viewport->x -= viewport_speed;
    if (keys[SDL_SCANCODE_S]) viewport->y += viewport_speed;
    if (keys[SDL_SCANCODE_D]) viewport->x += viewport_speed;

    return 0;
}

int init_SDL(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height) 
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) 
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow(
            "The gaming game",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN);
    if (!window) 
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) 
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    return 0;
}
