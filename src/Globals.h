#ifndef GLOBALS_H
#define GLOBALS_H
#include "Window.cpp"
Vector canvas(720, 720);
float const d = 1.f;
Vector D = {1,1,1};
float vpWidth = 1.f;
float vpHeight = 1.778f;
//only rendering when frame change to make sure my cpu doesn't explode
//Especially for Ray tracer
bool change = true;
//true = raytracer , false = rasterizer
bool rendMode = false;
//100fps
double frameLimit = 1.66f;
double FOV = 60;
Transform camera(Vector{ 0,3,0 }, 1, {0,0,0});
Scene scene = {};
//temp
Triangle tempTri;

#endif