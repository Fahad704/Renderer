#pragma once
#include "Window.cpp"
#define isDown(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)
bool rendMode=false;
//Back face culling
bool bfc = true;
//frame delta time
double fdt=0.06;
void init() {
	debugState = DebugState::DS_TRIANGLE;
	//Spheres (currently only rendered in ray tracing)
	std::vector<Sphere> spheres = {}; 
	//Sphere spherestem[] = {
		//	//Position-----Radius---Colour----specular-reflect//
			//{ { 0 , 0  , 3}, 1  ,{255,0 , 0}	,500,  0.2f},
			//{ { 2 ,  0  , 4}, 1  ,{0, 0, 255}	,500,  0.4f },
			//{ { -2, 0   , 4}, 1  ,{0, 255, 0}	,10,   0.3f},
		   // {{ 1, 1   , 0}, 0.5f  ,{0, 255, 0}	,10,   0.3f},
			//{ { 0 ,-5001, 0},5000,{255,125,0}	,1000, 0.5f}
	//};
	//int spherecount = sizeof(spherestem) / sizeof(Sphere);
	//for (int i = 0; i < spherecount; i++) {
	//	spheres.push_back(spherestem[i]);
	//}

	std::vector<Light> lights = {
		//--Type----------Pos----Dir--intensity//
		{LT_AMBIENT    ,{0,0,0},{0,0,0},0.2f},
		{LT_POINT      ,{2,1,0},{0,0,0},0.6f},
		{LT_DIRECTIONAL,{0,0,0},{1,4,4},0.2f}
	};

	std::vector<Triangle> triangles = {};
	std::vector<Mesh> meshes = {};

	Mesh King = loadOBJ("../Models/King.obj", { 0,-0.8f,3 }, { 255,255,255 }, 0.9f,1000);
	meshes.push_back(King);

	scene = { spheres,triangles,meshes,lights };
}
void update(Input* input) {
	timer::Timer start = timerStart();
	double speed = 5.f;
	Vector move = { 0,0,0 };
	//Movement events
	if (isDown(BUTTON_RIGHT)) 
		move = Vector{ 1.f, 0, 0 };
	
	if (isDown(BUTTON_LEFT)) 
		move = move + Vector{ -1.f, 0, 0 };
	
	if (isDown(BUTTON_UP)) 
		move = move + Vector{ 0, 0, 1.f };
	
	if (isDown(BUTTON_DOWN)) 
		move = move + Vector{ 0, 0, -1.f };
	
	if (isDown(BUTTON_W)) 
		move = move + Vector{ 0,1.f,0 };
	
	if (isDown(BUTTON_S)) 
		move = move + Vector{ 0,-1.f,0 };
	//Normalize move vector and move the Camera
	if (!(move == Vector{ 0,0,0 })) {

		move = move / length(move);
		move = move * speed;
		//O = O + move * fdt;
		scene.meshes[0].setPos((scene.meshes[0].getPos() + move * fdt));
	}
	//Show triangles of the mesh
	if (isDown(BUTTON_T)) 
		debugState = DebugState::DS_TRIANGLE;
	//Show bounding box of the mesh
	if (isDown(BUTTON_B)) 
		debugState = DebugState::DS_BOUNDING_BOX;
	//Turn off Debug view
	if (isDown(BUTTON_V)) 
		debugState = DebugState::DS_OFF;
	//Change ray tracing to rasterization and vise versa
	if (pressed(BUTTON_R))
		rendMode = !rendMode;
	if (pressed(BUTTON_P))
		exportToPPM("Image.ppm");
	if (pressed(BUTTON_C))
		bfc = !bfc;
	
	//Ray trace
	clearScreen(0x000000);
	if (rendMode){
		//Ray tracing with 4 threads
		size_t threadCount = 4;
		std::vector<std::thread> tObjs(threadCount);
		for (size_t i = 0; i < threadCount; i++)
		{
			tObjs[i] = std::thread(rayTraceThr, i, threadCount);
		}
		for (size_t i = 0; i < tObjs.size(); i++) {
			tObjs[i].join();
		}
		std::time_t endTime;
		timer::Duration deltaTime = timer_end(start, endTime);
		fdt = deltaTime.count();
		std::cout << std::fixed << "\r" << 1.f / (deltaTime.count()) << " FPS" << std::flush;
		return;
	}


	//Rasterize
	renderObject(scene.meshes[0],bfc);
	std::time_t endTime;
	//Limit frame rate to reduce power consumption
	double overhead = (frameLimit * 100) - (fdt);
	if (overhead > 0) {
		Sleep(overhead);
	}
	timer::Duration deltaTime = timer_end(start,endTime);
	fdt = deltaTime.count();
	//Display fps on console
	std::cout <<std::fixed<<"\r" <<1.f/(deltaTime.count()) << " FPS" << std::flush;
}