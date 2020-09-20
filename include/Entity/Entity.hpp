/*******************************************************************************************
*   Entity.hpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#ifndef MECHA_ENTITY_HPP
#define MECHA_ENTITY_HPP

#include <string>

#include "MPhysac/MPhysacBody.hpp"
#include "Utils/Vector.hpp"

class Entity {
    public:
        bool getIsPassable(){return isPassable;};

        bool getIsSpawned(){return isSpawned;};

        void setPosition(Vector2f newPos){position = newPos;}
        Vector2f getPosition(){return position;}

        void spawn();
        void despawn();

        Entity();
        ~Entity();

    protected:
        bool isPassable;        // If the Entity is passable

    private:
        bool isSpawned;         // If the Entity is spawned
        Vector2f position;
        std::string textureName;
        //Texture2D texture;
};

#endif