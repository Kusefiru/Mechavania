/*******************************************************************************************
*   PlayerEntity.hpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#include "Entity.hpp"

class PlayerEntity : public Entity
{
    public:
        int getLifePoints();
        int setLifePoints();
        int getOxygenLevel();
        int setOxygenLevel();

        PlayerEntity(Vector2 pos, int width, int height, int density, std::string textureName);

    private:
        int lifePoints;
        int oxygenLevel;
};