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
        PhysicsBody& getPhysicsBody();

        bool getIsPassable();
        void setIsPassable(bool newIsPassable);

        void setTexture(std::string newTextureName);

        Entity(Vector2 pos, int width, int height, int density, std::string textureName);
        ~Entity();

    private:
        PhysicsBody body;
        bool isPassable;
        std::string textureName;
        Texture2D texture;
};

#endif