/*******************************************************************************************
*   Libs.hpp
*
*   Lists the different libs used
*
********************************************************************************************/

#ifndef MECHA_LIBS_HPP
#define MECHA_LIBS_HPP

extern "C"{
    #include "raylib.h"
    #define PHYSAC_IMPLEMENTATION
    #define PHYSAC_NO_THREADS
    #define PHYSAC_STATIC
    #include "physac.h"
}

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#endif