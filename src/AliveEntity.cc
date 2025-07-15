#include "AliveEntity.hh"

void AliveEntity::damage(int amount)
{
    if (health - amount <= 0)
    {
        health = -1;
        return;
    }

    health -= amount;
}

void AliveEntity::heal(int amount)
{
  // -1 in max_health means 'no limit' and if you bypass max_health by some
  // event or likewise you should be able to keep growing undeadlier.
  //
  // aka: not a bug; a feature!
  if (health + amount > max_health && max_health > health && max_health != -1) {
    health = max_health;
    return;
  }

    health += amount;
}

void AliveEntity::draw()
{
    SDL_Rect r{ 
        int(worldPosition.x - viewport->x),
        int(worldPosition.y - viewport->y),
        int(dimensions.x), int(dimensions.y)
    };

    // if (i == 0) cout << "[ enemy" << i << " ] \t\t x: " << x << " y: " << y << endl;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

int AliveEntity::get_hp()
{
    return health;
}
