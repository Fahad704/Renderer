#ifndef GLOBALS_H
#define GLOBALS_H
#include "Window.cpp"
#include "Vector.h"


Vector canvas(720, 720);
//less = more FOV , more = lesser FOV
const float d = 0.52f;
const float farDist = 100.f;
Vector D = { 1,1,1 };
float vpWidth = 1.f;
float vpHeight = 1.f;
//only rendering when frame change to make sure my cpu doesn't explode
//Especially for Ray tracer
bool change = true;
//true = raytracer , false = rasterizer
bool rendMode = false;

//144 fps
float frameLimit = 6.94444444f;
double FOV = 90;
Transform camera(Vector{ 0,0,0 }, 1, { 0,0,0 });
Scene scene = {};
//temp
Triangle tempTri;
Plane planes[6] = {
	//Near
	{ {0, 0, 1},-d },
	//far
	{ {0, 0, -1},farDist },
	//Right
	{ {float(-1 / sqrt(2)),0,float(1 / sqrt(2))},0.f },
	//Left
	{ {float(1 / sqrt(2)),0,float(1 / sqrt(2))},0.f },
	//Up
	{ {0,float(-1 / sqrt(2)),float(1 / sqrt(2))},0.f },
	//Down
	{ {0,float(1 / sqrt(2)),float(1 / sqrt(2))},0.f }
};

#endif