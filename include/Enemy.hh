#ifndef ENEMY_H
#define ENEMY_H

#include "AliveEntity.hh"
#include <iostream>

using namespace std;

class Enemy: public AliveEntity
{
    private:
        int attackDamage;

    public:
        Enemy(int id, SDL_Renderer* renderer, Vector* viewport, int attackDamage_)
          : AliveEntity(id, renderer, viewport)
          , attackDamage(attackDamage_)
    {
        cout << "Enemy created, entity id: " << id << endl;
    }
};

#endif
