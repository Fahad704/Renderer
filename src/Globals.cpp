#ifndef GLOBALS_CPP
#define GLOBALS_CPP
#include "Window.cpp"
Vector canvas(720, 720);
int triSeenCount = 0;
float const d = 1.f;
Vector D = {1,1,1};
float vpWidth = 1;
float vpHeight = 1.778;
bool once = true;
//100fps
double frameLimit = 1.66f;
double FOV = 60;
Transform camera(Vector{ 0,0,0 }, 1, {0,0,0});
Scene scene = {};
//temp
Triangle tempTri;
DebugState debugState;
#endif