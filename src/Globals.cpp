#ifndef GLOBALS_CPP
#define GLOBALS_CPP
#include "Window.cpp"
Vector canvas(720, 720);
float const d = 1.f;
Vector D = {1,1,1};
float vpWidth = 1;
float vpHeight = 1.778;
//100fps
double frameLimit = 0.0125f;
Transform camera(Vector{ 1.5,0,-3 }, 1, {0,0,0});
Scene scene = {};
//temp
Triangle tempTri;
DebugState debugState;
#endif