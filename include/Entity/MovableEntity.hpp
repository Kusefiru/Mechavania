/*******************************************************************************************
*   MovableEntity.hpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#ifndef MECHA_MOVABLEENTITY_HPP
#define MECHA_MOVABLEENTITY_HPP

#include "Entity.hpp"

class MovableEntity : public Entity {
    public:
        MPhysacBody* body;  // A movable entity has a MPhysacBody

        MovableEntity();
        ~MovableEntity(){};

    private:
};

#endif