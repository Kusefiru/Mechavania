/*******************************************************************************************
*   PlayerEntity.hpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#ifndef MECHA_PLAYERENTITY_HPP
#define MECHA_PLAYERENTITY_HPP

#include "MovableEntity.hpp"
#include "Drawable.hpp"
#include "Sprite.hpp"
#include "MPhysac/MPhysacBody.hpp"

class PlayerEntity : public MovableEntity, public Drawable {
    public:
        int getLifePoints(){return lifePoints;}
        void setLifePoints(int newVal){lifePoints = newVal;}
        int getOxygenLevel(){return oxygenLevel;}
        void setOxygenLevel(int newVal){oxygenLevel = newVal;}

        void loadSprite(ResourcesLoader &rl);
        void draw(const Painter& painter) const {playerSprite.draw(painter, int(position.x), int(position.y));}

        PlayerEntity(ResourcesLoader& rl);
        ~PlayerEntity(){};

        Vector2f position;

    private:
        int lifePoints;
        int oxygenLevel;
        Sprite playerSprite;
        MPhysacBody *playerBody;
};

#endif