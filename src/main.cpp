#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
// #include <fmt/core.h>
#include <flecs.h>
#include <unordered_map>
// #include "../lib/flecs/include/flecs.h"

#include "utils.hpp"

// Singleton(-ish) structs
struct PlayerInput_Movement
{
    bool sprint;
    bool up;
    bool down;
    bool left;
    bool right;
};

struct PlayerInput
{
    PlayerInput_Movement Movement;
};

struct Dimensions
{
    int x;
    int y;
};

enum BlockType
{
    DIRT,  // {101, 67, 33}
    STONE, // { 60, 60, 60}
    ROCK,  // { 48, 48, 48}
};

struct BlockPosition
{
    int x;
    int y;
    int z;
};

struct Block
{
    int id;
};

// Entity structs
struct Position 
{
    float x;
    float y;
};

struct Velocity 
{
    float x;
    float y;

    float sqrMagnitude() const 
    {
        return x*x + y*y;
    }

    void normalize() 
    {
        float length = std::sqrt(sqrMagnitude());
        if (length > 0.0f) 
        {
            x /= length;
            y /= length;
        }
    }
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
    Dimensions ViewportDimensions = {1280, 720};
    flecs::world* m_world;
    bool m_isRunning;
    std::unordered_map<std::string, SDL_Scancode> m_keybinds = {
        { "sprint",  SDL_SCANCODE_LSHIFT    },
        { "up",      CharToSDLScancode('w') },
        { "down",    CharToSDLScancode('s') },
        { "left",    CharToSDLScancode('a') },
        { "right",   CharToSDLScancode('d') },
        { "quit",    SDL_SCANCODE_ESCAPE    }
    };
    const Uint8* keyboardState;


public:
    Game(flecs::world* world_) : m_world(world_), m_isRunning(true) 
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) 
        {
            std::cout << "SDL_Init Error: {}" << SDL_GetError();
            m_isRunning = false;
            return;
        }

        m_window = SDL_CreateWindow(
            "Oatmeal",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            ViewportDimensions.x,   // width, 
            ViewportDimensions.y,   // height 
            SDL_WINDOW_SHOWN        // flags
        );

        if (!m_window) 
        {
            std::cout << "SDL_CreateWindow Error: {}" << SDL_GetError();
            m_isRunning = false;
            SDL_Quit();
            return;
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_renderer) 
        {
            std::cout << "SDL_CreateRenderer Error: {}" << SDL_GetError();
            m_isRunning = false;
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return;
        }

        SDL_PumpEvents();
        keyboardState = SDL_GetKeyboardState(nullptr);

        setup_systems();
        create_entities();

    }

    ~Game() 
    {
        std::cout << "Exiting game...\n";
        clean();
        std::cout << "Game terminated.\n";
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
            .kind(flecs::OnUpdate)
            .each([this](Velocity& v, PlayerInput& pi)
            {

                Velocity mono_vel = {0, 0};

                if (pi.Movement.up)    mono_vel.y += -1.0f;
                if (pi.Movement.down)  mono_vel.y +=  1.0f;
                if (pi.Movement.left)  mono_vel.x += -1.0f;
                if (pi.Movement.right) mono_vel.x +=  1.0f;

                if (mono_vel.sqrMagnitude() > 0.0f)
                    mono_vel.normalize();

                float speed = pi.Movement.sprint ? 550.0f : 300.0f;

                // chose the faster speed
                v.x = std::abs(mono_vel.x * speed) > std::abs(v.x) 
                    ? mono_vel.x * speed
                    : v.x;

                v.y = std::abs(mono_vel.y * speed) > std::abs(v.y) 
                    ? mono_vel.y * speed
                    : v.y;


                pi.Movement.sprint  =  false;
                pi.Movement.up      =  false;
                pi.Movement.down    =  false;
                pi.Movement.left    =  false;
                pi.Movement.right   =  false;

            });

        // -- Movement System --
        m_world->system<Position, Velocity>("MovementSystem")
            .kind(flecs::OnUpdate)
            .each([](flecs::iter& it, size_t index, Position& p, Velocity& v) 
            {
                p.x += v.x * it.delta_time();
                p.y += v.y * it.delta_time();

                v.x *= 0.97f;
                v.y *= 0.97f;
            });

        // -- Render System --
        m_world->system<const Position, RenderableSquare>("RenderSystem")
            .kind(flecs::PostUpdate) // Run after the default OnUpdate systems
            .each([this](const Position& p, RenderableSquare& rs) 
            {
                rs.rect.x = static_cast<int>(p.x);
                rs.rect.y = static_cast<int>(p.y);

                auto viewport = m_world->lookup("Viewport");
                auto view_pos = viewport.get<Position>();

                SDL_Rect body;

                body.x = rs.rect.x - view_pos.x 
                                   + ViewportDimensions.x / 2
                                   - rs.rect.w / 2;
                body.y = rs.rect.y - view_pos.y 
                                   + ViewportDimensions.y / 2
                                   - rs.rect.h / 2;
                body.w = rs.rect.w;
                body.h = rs.rect.h;

                SDL_SetRenderDrawColor(m_renderer, rs.color.r, rs.color.g, rs.color.b, rs.color.a);
                SDL_RenderFillRect(m_renderer, &body);
            });
    }

    auto blockBuilder(int x, int y, int z, BlockType type)
    {
        auto block = m_world->entity()
            .set<BlockPosition>({x, y, z})
            .set<BlockType>({type});

        return block;
    };

    void create_entities() 
    {
        m_world->entity("Player")
            .set<Position>({ 400, 300 })
            .set<Velocity>({ 0, 0 })
            .set<RenderableSquare>({ {0, 0, 90, 90}, {0, 255, 0, 224} }) // White
            .set<PlayerInput>({{ false, false, false, false }});

        m_world->entity("Viewport")
            .set<Position>({ 400, 300 });

        for (int i = 0; i < 10; i++) 
        {
            m_world->entity()
                .set<Position>({ ((float)i * 20.0f + 400.0f), 50.0f }) // random
                .set<Velocity>({ 0.0f, 0.0f })                      // random just for fun
                .set<RenderableSquare>({ {0, 0, i * 20, i * 20}, {255, 0, 0} }); // Enemy = Red
        }
    }

    void handle_playerInputEvents()
    {
        auto player = m_world->lookup("Player");
        if (!player) return;
        PlayerInput& pi = player.get_mut<PlayerInput>();

        pi.Movement.sprint  |= keyboardState[m_keybinds["sprint"]];
        pi.Movement.up      |= keyboardState[m_keybinds["up"]];
        pi.Movement.down    |= keyboardState[m_keybinds["down"]];
        pi.Movement.left    |= keyboardState[m_keybinds["left"]];
        pi.Movement.right   |= keyboardState[m_keybinds["right"]];
    };

    void handle_events() 
    {
        SDL_Event event;
        SDL_PumpEvents();

        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
                m_isRunning = false;
        }

        if (keyboardState[m_keybinds["quit"]]) m_isRunning = false;
        handle_playerInputEvents();
        // TODO: 
        // [x] Figure out where to store the PlayerInput struct.
        //      ans: in the ecs
        // [x] Take input and assert values to the PlayerInput struct.
        // [x] Make the system for input.


    }
    void updateViewportPosition()
    {
        auto player = m_world->lookup("Player");
        auto viewport = m_world->lookup("Viewport");

        Position& viewportPos = viewport.get_mut<Position>();
        const Position playerPos = player.get<Position>();

        std::cout << "viewport | x: " << viewportPos.x << " | y: " << viewportPos.y << std::endl;

        viewportPos.x = playerPos.x;
        viewportPos.y = playerPos.y;

        std::cout << "player   | x: " << playerPos.x << "   | y: " << playerPos.y << std::endl;
    };

    void update() 
    {
        updateViewportPosition();
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
        if (m_renderer) SDL_DestroyRenderer(m_renderer);
        if (m_window) SDL_DestroyWindow(m_window);
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
