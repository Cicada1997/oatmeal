#ifndef PLAYER_H
#define PLAYER_H

#include "AliveEntity.hh"
#include <iostream>
#include <Debug.hh>

using namespace std;

class Player: public AliveEntity
{
    private:
        string username;


    public:
        Player(int id, SDL_Renderer* renderer, Vector* viewport, string username_)
            : AliveEntity(id, renderer, viewport)
            , username(username_)
        {
            cout << "Player created "<< username << " at x: " << worldPosition.x << " y: " << worldPosition.y << " ." << endl;
        }

        ~Player()
        {
            cout << "Player " << username << " deleted." << endl;
        }

        void draw();
        int get_hp();
};

#endif
