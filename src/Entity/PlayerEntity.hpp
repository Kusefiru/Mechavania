/*******************************************************************************************
*   PlayerEntity.hpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#ifndef MECHA_PLAYERENTITY_HPP
#define MECHA_PLAYERENTITY_HPP

#include "Entity.hpp"

class PlayerEntity : public Entity
{
    public:
        int getLifePoints(){return lifePoints;}
        void setLifePoints(int newVal){lifePoints = newVal;}
        int getOxygenLevel(){return oxygenLevel;}
        void setOxygenLevel(int newVal){oxygenLevel = newVal;}

        PlayerEntity(const Vector2f &pos, int width, int height, int density, const std::string &textureName);

    private:
        int lifePoints;
        int oxygenLevel;
};

#endif