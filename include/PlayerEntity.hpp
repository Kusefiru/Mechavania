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
        int GetLifePoints();
        int SetLifePoints();
        int GetOxygenLevel();
        int SetOxygenLevel();

        PlayerEntity();
        ~PlayerEntity();

    private:
        int LifePoints;
        int OxygenLevel;
};