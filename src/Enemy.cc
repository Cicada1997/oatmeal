#include "Enemy.hh"

void attack();
void findTarget();
void lifeloop();
// Follows Target
void follow();

// Attack the target if in range
void Enemy::attack()
{
    if ( CurrentTarget == nullptr || CurrentTarget->distance(&this->worldPosition) > view_distance) 
        return;
};

void Enemy::follow()
{
    this->applyForce(CurrentTarget->getWorldPosition() - this->worldPosition);
};

// Seek target via distance formula. EXTRA: Collision
void Enemy::findTarget(vector<AliveEntity*>* targets_)
{
    this->Targets = targets_;
};

// Called by update(); overwritten
void Enemy::lifeloop(double dt)
{
    if (CurrentTarget == nullptr)
        findTarget(Targets);
}
