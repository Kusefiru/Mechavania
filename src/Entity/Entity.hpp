/*******************************************************************************************
*   Entity.hpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#ifndef MECHA_ENTITY_HPP
#define MECHA_ENTITY_HPP

#include "Libs.hpp"

class Entity 
{
    public:
        MPhysacBody* body;

        bool getIsPassable();
        void setIsPassable(bool newIsPassable);

        bool getIsSpawned();
        void spawn();
        void despawn();

        void setTexture(std::string newTextureName);

        Entity(const Vector2f &pos, int width, int height, int density, std::string textureName);
        ~Entity();

    private:
        bool isPassable;        // If the Entity is passable by the player / the ennemis
        bool isSpawned;         // If the Entity is spawned
        std::string textureName;
        //Texture2D texture;
};

#endif