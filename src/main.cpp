#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
// #include <fmt/printf.h>
#include <flecs.h>
// #include "../lib/flecs/include/flecs.h"

struct PlayerInput_Movement
{
    bool left;
    bool right;
    bool up;
    bool down;
};

struct PlayerInput
{
    PlayerInput_Movement Movement;
};

struct Position 
{
    float x;
    float y;
};

struct Velocity 
{
    float x;
    float y;
};

struct RenderableSquare 
{
    SDL_Rect rect;
    SDL_Color color;
};

class Game 
{
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    flecs::world* m_world;
    bool m_isRunning;

public:
    Game(flecs::world* world_) : m_world(world_), m_isRunning(true) 
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) 
        {
            // fmt::printf("SDL_Init Error: {}", SDL_GetError());
            m_isRunning = false;
            return;
        }

        m_window = SDL_CreateWindow(
            "Flecs & SDL2 Example", // window title
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280, 720,             // width, height
            SDL_WINDOW_SHOWN       // flags
        );

        if (!m_window) 
        {
            // fmt::printf("SDL_CreateWindow Error: {}", SDL_GetError());
            m_isRunning = false;
            SDL_Quit();
            return;
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_renderer) 
        {
            // fmt::printf("SDL_CreateRenderer Error: {}", SDL_GetError());
            m_isRunning = false;
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return;
        }

        setup_systems();
        create_entities();
    }

    ~Game() 
    {
        // fmt::printf("Exiting game...\n");
        clean();
        // fmt::printf("Game terminated.\n");
    }

    void run() 
    {
        while (m_isRunning) 
        {
            handle_events();
            update();
            render();
        }
    }

private:
    void setup_systems() 
    {
        m_world->system<Velocity, PlayerInput>("PlayerInput")
            .each([this](Velocity& v, PlayerInput& pi)
            {
                if (pi.Movement.left)  v.x -= 100;
                if (pi.Movement.right) v.x += 100;
                if (pi.Movement.up)    v.y -= 100;
                if (pi.Movement.down)  v.y += 100;
            });
        // -- Movement System --
        m_world->system<Position, const Velocity>("MovementSystem")
            .each([](flecs::iter& it, size_t index, Position& p, const Velocity& v) 
            {
                p.x += v.x * it.delta_time();
                p.y += v.y * it.delta_time();
            });

        // -- Render System --
        m_world->system<const Position, RenderableSquare>("RenderSystem")
            .kind(flecs::PostUpdate) // Run after the default OnUpdate systems
            .each([this](const Position& p, RenderableSquare& rs) 
                    {

                rs.rect.x = static_cast<int>(p.x);
                rs.rect.y = static_cast<int>(p.y);

                SDL_SetRenderDrawColor(m_renderer, rs.color.r, rs.color.g, rs.color.b, rs.color.a);
                SDL_RenderFillRect(m_renderer, &rs.rect);
            });
    }

    void create_entities() 
    {
        m_world->entity("Player")
            .set<Position>({ 400, 300 })
            .set<Velocity>({ 50, 0 }) // Give it some velocity to see it move
            .set<RenderableSquare>({ {0, 0, 90, 90}, {255, 255, 255, 255} }); // White

        for (int i = 0; i < 10; i++) 
        {
            m_world->entity()
                .set<Position>({ (float)i * 100.0f, (float)i * 50.0f })
                .set<Velocity>({ -30.0f * (i + 1), 60.0f * (i + 1) })
                .set<RenderableSquare>({ {0, 0, 20, 20}, {(Uint8)(i * 25), (Uint8)(255 - i * 25), 150, 255} }); // very cool
        }
    }

    void handle_events() 
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
                m_isRunning = false;

            if (event.type == SDL_KEYDOWN) 
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    m_isRunning = false;

                // TODO: 
                // 1. Figure out where to store the PlayerInput struct.
                // 2. Take input and assert values to the PlayerInput struct.
                // 3. Make the system for input.


            }
        }
    }

    void update() 
    {
        m_world->progress(); // runs all systems
    }

    void render() 
    {
        // im fuckking retarded:
        // SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
        // SDL_RenderClear(m_renderer);

        SDL_RenderPresent(m_renderer);
        SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
        SDL_RenderClear(m_renderer);
    }

    void clean() 
    {
        if (m_renderer) 
            SDL_DestroyRenderer(m_renderer);

        if (m_window) 
            SDL_DestroyWindow(m_window);

        SDL_Quit();
    }
};

// -- Main Function --
int main(int argc, char* argv[]) 
{
    flecs::world world;
    Game game(&world);
    game.run();
    return 0;
}
