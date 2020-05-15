/*******************************************************************************************
*   Entity.hpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#ifndef MECHA_ENTITY_HPP
#define MECHA_ENTITY_HPP

#include <string>
#include "Libs.hpp"

class Entity 
{
    public:
        PhysicsBody getPhysicsBody();

        bool getIsPassable();
        setIsPassable(bool newIsPassable);

        setTexture(std::string newTextureName);

        Entity(Vector2 pos, int width, int height, int density, std::string textureName);
        ~Entity();

    private:
        PhysicsBody body;
        bool isPassable;
        std::string textureName;
        Texture2D texture;
};

#endif