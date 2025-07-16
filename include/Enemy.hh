#ifndef ENEMY_H
#define ENEMY_H

#include "AliveEntity.hh"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "cstdlib"

using namespace std;

class Enemy: public AliveEntity
{
    protected:
        int attack_damage;
        int view_distance;
        vector<AliveEntity*>* Targets;
        AliveEntity* CurrentTarget;

        // Attack the target if in range
        void attack();
        // Seek target via distance formula. EXTRA: Collision
        void findTarget();
        // Called by update(); overwritten
        void lifeloop(double dt);
        // Follows Target
        void follow();

    public:
        Enemy(int id, SDL_Renderer* renderer, Vector* viewport, int attackDamage_)
          : AliveEntity(id, renderer, viewport)
          , attack_damage(attackDamage_)
    {
        // red:
        int rgb[] = {255, 0, 0};

        copy(begin(rgb), end(rgb), begin(color));
        // color[0] = 255; color[1] = 0; color[2] = 0;

        cout << "Enemy created, entity id: " << id << endl;
    };

        // When new entities loaded, let everyone know
        void findTarget(vector<AliveEntity*>* targets_);
};

#endif
