#ifndef GLOBALS_CPP
#define GLOBALS_CPP
#include "Window.cpp"
Vector canvas(720, 720);
SceneSettings sceneSettings = {true,true,0,DebugState::DS_TRIANGLE};
float const d = 1.f;
Vector D = {1,1,1};
float vpWidth = 1.f;
float vpHeight = 1.778f;
bool once = true;
//100fps
double frameLimit = 1.66f;
double FOV = 60;
Transform camera(Vector{ 0,0,0 }, 1, {0,0,0});
Scene scene = {};
//temp
Triangle tempTri;
#endif