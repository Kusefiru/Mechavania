/*******************************************************************************************
*   MCamera.hpp
*
*   This file designs the MCamera class for the game camera
*
********************************************************************************************/

#ifndef MECHA_CAMERA_HPP
#define MECHA_CAMERA_HPP

#include "MPhysac/MPhysacBody.hpp"

class MCamera {
    public:
        void update();
        void setFollowedPhysicsBody(MPhysacBody* body);
        //Camera2D& getCamera();

        MCamera();

    private:
        //Camera2D camera;
        MPhysacBody* cameraPhysicsBody;
        MPhysacBody* followedPhysicsBody;
};

#endif