#ifndef GLOBALS_H
#define GLOBALS_H
//#include "Window.cpp"
#include <math.h>
#include "Window.h"
#include "Object.h"
#include "SceneSettings.h"
#include "Transform.h"
#include "Scene.h"

extern Window window;// = {};
extern void* depth;
extern RenderState renderState;
extern bool running;// = true;
extern SceneSettings sceneSettings;// = { true,false,0,DebugState::DS_OFF, true ,RenderMode::RM_COLOR };
extern Vector canvas;// (720, 720);
//less = more FOV , more = lesser FOV
const float d = 0.52f;
const float farDist = 1000.f;
extern Vector D;// = { 1,1,1 };
extern float vpWidth;// = 1.f;
extern float vpHeight;// = 1.f;
//only rendering when frame change to make sure my cpu doesn't explode
//Especially for Ray tracer
extern bool change;// = true;
//true = raytracer , false = rasterizer
extern bool rayTraceMode;// = false;

//144 fps
extern float frameLimit;// = 6.94444444f;
extern double FOV;// = 90;
extern Transform camera;//(Vector{ 0,0,0 }, 1, { 0,0,0 });
extern Scene scene;// = {};
//temp
extern Triangle tempTri;
extern Plane planes[6];// = {
	//Near
//	{ {0, 0, 1},-d },
//	//far
//	{ {0, 0, -1},farDist },
//	//Right
//	{ {float(-1 / sqrt(2)),0,float(1 / sqrt(2))},0.f },
//	//Left
//	{ {float(1 / sqrt(2)),0,float(1 / sqrt(2))},0.f },
//	//Up
//	{ {0,float(-1 / sqrt(2)),float(1 / sqrt(2))},0.f },
//	//Down
//	{ {0,float(1 / sqrt(2)),float(1 / sqrt(2))},0.f }
//};

#endif