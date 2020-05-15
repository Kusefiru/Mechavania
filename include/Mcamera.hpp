/*******************************************************************************************
*   Mcamera.hpp
*
*   This file designs the Mcamera class for the game camera
*
********************************************************************************************/

#ifndef MECHA_CAMERA_HPP
#define MECHA_CAMERA_HPP

#include "Libs.hpp"

class Mcamera
{
    public:
        void update();
        void setFollowedPhysicsBody(PhysicsBody* body);
        Camera2D& getCamera();

        Mcamera();

    private:
        Camera2D camera;
        PhysicsBody cameraPhysicsBody;
        PhysicsBody* followedPhysicsBody;
};

#endif