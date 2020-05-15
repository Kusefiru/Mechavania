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
        PhysicsBody GetPhysicsBody();

        bool GetIsPassable();
        SetIsPassable(bool NewIsPassable);

        SetTexture(std::string NewTextureName);

        Entity(Vector2 Pos, int Width, int Height, int Density, std::string TextureName);
        ~Entity();

    private:
        PhysicsBody Body;
        bool IsPassable;
        std::string TextureName;
        Texture2D Texture;
};

#endif