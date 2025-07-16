#include "PObject.hh"
#include "Debug.hh"
#include "Vector.hh"
#include <SDL2/SDL_render.h>
#include <cmath>
#include <iostream>

using namespace std;

PObject::PObject(int id_, SDL_Renderer* renderer_, Vector* viewport_)
    : id(id_)
    , renderer(renderer_)
    , viewport(viewport_)
    , worldPosition{0, 0}
    , velocity{0, 0}
    , mass(2.4)
    , terminal_velocity(400.0)
    , friction_modifier(3.5)
    , dimensions{50, 50}
    , color{50, 50, 50}
{
    cout << "Created PObject '" << id << "'\n";
}

PObject::~PObject() 
{
    cout << "Destroyed PObject '" << id << "'\n";
}

void PObject::applyForce(const Vector& force) 
{
    // F = m * a  →  a = F / m
    if (mass != 0 || force.x != 0 || force.y != 0)
        acceleration += force / mass;
}

void PObject::setTerminalVelocity(double tv) { terminal_velocity = tv; }
void PObject::setWorldPosition(Vector xy) { worldPosition = xy; }

double PObject::distance(Vector* position)
{
    Vector distanceVector  = (this->worldPosition - *position);

    double distance        = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);

    return distance;
}

double PObject::getSpeed() const 
{
    return sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
}

void PObject::lifeloop(double dt)
{
    cerr << "Lifeloop not configured for entity with id '" << id << "'!" << endl;
}

void PObject::update(double dt) 
{
    velocity += acceleration * dt;

    double speed = getSpeed();
    if (speed > terminal_velocity && speed != 0 && terminal_velocity != 0) 
    {
        double scale = terminal_velocity / speed;
        velocity *= scale;
    }

    velocity -= velocity * (friction_modifier * dt);

    worldPosition += velocity * dt;

    acceleration = {0, 0};

    // debug << "  pos=(" << worldPosition.x << "," << worldPosition.y
    // << ") vel=(" << velocity.x << "," << velocity.y << ")\n";
}
