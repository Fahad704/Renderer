#ifndef GLOBALS_H
#define GLOBALS_H
#include "Window.cpp"



Vector canvas(720, 720);
//less = more FOV , more = lesser FOV
const float d = 0.52f;
Vector D = {1,1,1};
float vpWidth = 1.f;
float vpHeight = 1.f;
//only rendering when frame change to make sure my cpu doesn't explode
//Especially for Ray tracer
bool change = true;
//true = raytracer , false = rasterizer
bool rendMode = false;

//1ms
float frameLimit = 1;
double FOV = 90;
Transform camera(Vector{ 0,0,0 }, 1, {0,0,0});
Scene scene = {};
//temp
Triangle tempTri;
Plane planes[5] = {
	//Near
	{ {0, 0, 1},d },
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