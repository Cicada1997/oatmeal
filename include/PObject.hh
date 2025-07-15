#ifndef PObject_H
#define PObject_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <string>
#include "Vector.hh"

using namespace std;

// Physics Object
class PObject 
{
    public:
        PObject(int id_, SDL_Renderer* renderer_, Vector* viewport_);
        ~PObject();

        // apply an instant force (F = m * a → a = F/m)
        void applyForce(const Vector& force);
        void setTerminalVelocity(double tv);
        void setWorldPosition(Vector xy);

        // simulate one time‐slice of dt seconds
        void update(double dt);

        Vector  getWorldPosition() const { return worldPosition; }
        Vector  getDimensions() const { return dimensions; }
        double  getSpeed()    const;

    protected:
        string name;
        int    id;

        // Physical data
        Vector worldPosition;
        Vector velocity;
        Vector acceleration = {0, 0};

        double mass              = 2.4;
        double terminal_velocity = 200.0; // units/sec
        double friction_modifier = 1.5;   // drag coefficient

        // Graphical data
        Vector dimensions = {50, 50};
        int    color[3];

        SDL_Renderer* renderer;
        Vector* viewport;

};

#endif
