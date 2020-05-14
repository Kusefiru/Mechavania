/*******************************************************************************************
*   Entity.hpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#include "Libs.hpp"
#include <string>

class Entity 
{
    public:
        PhysicsBody GetPhysicsBody();
        SetTexture(std::string NewTextureName);
        Entity();
        ~Entity();

    private:
        PhysicsBody Body;
        std::string TextureName;
        Texture2D Texture;
};