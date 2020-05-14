/*******************************************************************************************
*   Entity.cpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#include "Entity.hpp"

class Entity {
    PhysicsBody GetPhysicsBody(){
        return Body;
    }

    SetTexture(std::string NewTextureName){
        TextureName = NewTextureName;
        // Update Texture
    }
}