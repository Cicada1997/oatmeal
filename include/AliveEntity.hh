#ifndef ALIVE_ENTITY_H
#define ALIVE_ENTITY_H

#include "PObject.hh"
#include <iostream>
#include <Debug.hh>

using namespace std;

class Item
{
    public:
        Item(int type_)
            : type(type_)
        { }

        int type;
};

class AliveEntity: public PObject
{
    protected:
        int health;
        int max_health;

    public:
        AliveEntity(int id, SDL_Renderer* renderer, Vector* viewport)
            : PObject(id, renderer, viewport)
        {
            health = max_health = 100;
            cout << "AliveEntity created "<< id << " at x: " << worldPosition.x << " y: " << worldPosition.y << " ." << endl;
        }

        ~AliveEntity()
        {
            cout << "AliveEntity " << id << " deleted." << endl;
        }

        void damage(int amount);
        void heal(int amount);
        void draw();
        int get_hp();
};

#endif
