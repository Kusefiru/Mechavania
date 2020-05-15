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

        PlayerEntity();
        ~PlayerEntity();

    private:
        int lifePoints;
        int oxygenLevel;
};