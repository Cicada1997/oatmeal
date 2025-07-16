#include "Enemy.hh"
#include "AliveEntity.hh"
#include <cstdlib>

// Attack the target (member) if in range
void Enemy::attack()
{
    if ( CurrentTarget == nullptr || CurrentTarget->distance(&this->worldPosition) > view_distance) 
        return;
};

// follows target (member)
void Enemy::follow()
{
    this->applyForce(CurrentTarget->getWorldPosition() - this->worldPosition);
};

// Seek target (member) via distance formula. EXTRA: Collision
void Enemy::findTarget(vector<AliveEntity*>* targets_)
{
    this->Targets = targets_;

    double CurrentTargetDistance = CurrentTarget->distance(&this->worldPosition);

    for (AliveEntity* indexTarget: *this->Targets)
    {
        if ( CurrentTargetDistance > indexTarget->distance(&this->worldPosition) )
        {
            CurrentTargetDistance = CurrentTarget->distance(&this->worldPosition);
            CurrentTarget = indexTarget;
        }
    }
};

// Called by update(); overwritten
void Enemy::lifeloop(double dt)
{
    if (CurrentTarget == nullptr)
        findTarget(Targets);
}
