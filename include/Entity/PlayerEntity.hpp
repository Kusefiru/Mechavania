/*******************************************************************************************
*   PlayerEntity.hpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#ifndef MECHA_PLAYERENTITY_HPP
#define MECHA_PLAYERENTITY_HPP

#include <chrono>

#include "MovableEntity.hpp"
#include "Drawable.hpp"
#include "AnimatedSprite.hpp"
#include "MPhysac/MPhysacBody.hpp"

class PlayerEntity : public MovableEntity, public Drawable {
    public:
        int getLifePoints(){return lifePoints;}
        void setLifePoints(int newVal){lifePoints = newVal;}
        int getOxygenLevel(){return oxygenLevel;}
        void setOxygenLevel(int newVal){oxygenLevel = newVal;}

        void move(const Vector2f &newPos) { position += newPos; playerSprite->setPosition((Vector2i)position); }
        void setPosition(const Vector2f &newPos) { position = newPos; playerSprite->setPosition((Vector2i)position); }
        void loadSprite(ResourcesLoader &rl);
        void draw(const Painter& painter) const { playerSprite->draw(painter); }

        void update(const std::chrono::duration<float> &dt) { playerSprite->update(dt); }

        PlayerEntity(ResourcesLoader& rl);
        ~PlayerEntity();

        Vector2f position;

    private:
        int lifePoints;
        int oxygenLevel;
        AnimatedSprite *playerSprite;
        MPhysacBody *playerBody;
};

#endif