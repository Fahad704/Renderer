#pragma once
#include "Window.cpp"
#define isDown(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)
bool change = true;
bool animation_state=0;
bool rendMode=false;
//frame delta time
double fdt=0.06;
void init() {
	debugState = DebugState::DS_OFF;
	std::vector<Sphere> spheres = {};
	Sphere spherestem[] = {
		//	//Position-----Radius---Colour----specular-reflect//
			//{ { 0 , 0  , 3}, 1  ,{255,0 , 0}	,500,  0.2f},
			//{ { 2 ,  0  , 4}, 1  ,{0, 0, 255}	,500,  0.4f },
			{ { -2, 0   , 4}, 1  ,{0, 255, 0}	,10,   0.3f},
		     // {{ 1, 1   , 0}, 0.5f  ,{0, 255, 0}	,10,   0.3f},
			{ { 0 ,-5001, 0},5000,{255,125,0}	,1000, 0.5f}
	};
	int spherecount = sizeof(spherestem) / sizeof(Sphere);
	for (int i = 0; i < spherecount; i++) {
		spheres.push_back(spherestem[i]);
	}
	std::vector<Light> lights = {
		//--Type----------Pos----Dir--intensity//
		{LT_AMBIENT    ,{0,0,0},{0,0,0},0.2f},
		{LT_POINT      ,{2,1,0},{0,0,0},0.6f},
		{LT_DIRECTIONAL,{0,0,0},{1,4,4},0.2f}
	};
	std::vector<Triangle> triangles = {};
	std::vector<Mesh> meshes = {};
	Mesh King = loadOBJ("Models/scene.obj");
	King.setPos({ 0,-0.8f,3 } ,Colour{ 255,255,255 });
	King.reflectiveness = 0.9f;
	King.specular = 1000;
	meshes.push_back(King);
	scene = { spheres,triangles,meshes,lights };
}
void update(Input* input) {
	timer::Timer start = timerStart();
	double speed = 5.f;
	//animate
	Vector move = { 0,0,0 };
	if (isDown(BUTTON_RIGHT)) {
		move = Vector{ 1.f, 0, 0 };
	}
	if (isDown(BUTTON_LEFT)) {
		move = move + Vector{ -1.f, 0, 0 };
	}
	if (isDown(BUTTON_UP)) {
		move = move + Vector{ 0, 0, 1.f };
	}
	if (isDown(BUTTON_DOWN)) {
		move = move + Vector{ 0, 0, -1.f };
	}
	if (isDown(BUTTON_W)) {
		move = move + Vector{ 0,1.f,0 };
	}
	if (isDown(BUTTON_S)) {
		move = move + Vector{ 0,-1.f,0 };
	}
	if (!(move == Vector{ 0,0,0 })) {

		move = move / length(move);
		move = move * speed;
		O = O + move * fdt;
	}
	if (isDown(BUTTON_T)) {
		debugState = DebugState::DS_TRIANGLE;
	}
	if (isDown(BUTTON_B)) {
		debugState = DebugState::DS_BOUNDING_BOX;
	}
	if (isDown(BUTTON_V)) {
		debugState = DebugState::DS_OFF;
	}
	if (pressed(BUTTON_R)) {
		rendMode = !rendMode;
	}
	//Ray trace
	clearScreen(0x000000);
	if (rendMode){
		size_t thread_count = 4;
		std::vector<std::thread> t_objs(thread_count);
		for (size_t i = 0; i < thread_count; i++)
		{
			t_objs[i] = std::thread(RayTraceThr, i, thread_count);
		}
		for (size_t i = 0; i < t_objs.size(); i++) {
			t_objs[i].join();
		}
		std::time_t endTime;
		timer::Duration deltaTime = timer_end(start, endTime);
		fdt = deltaTime.count();
		std::cout << std::fixed << "\r" << 1.f / (deltaTime.count()) << " FPS" << std::flush;
		return;
	}
		//RayTrace();


	//Rasterize
	//front vertices
	Vector cube[8];
	cube[3] = Vector{ -1,-1, 3 };
	cube[0] = Vector{ -1, 1, 3 };
	cube[1] = Vector{  1, 1, 3 };
	cube[2] = Vector{ 1,-1,  3 };

	//back vertices
	cube[7] = Vector{ -1,-1, 4 };
	cube[4] = Vector{ -1, 1, 4 };
	cube[5] = Vector{  1, 1, 4 };
	cube[6] = Vector{  1,-1, 4 };

	
	Triangle t;
	t.p[0] = Vector{ -1,1,3 };
	t.p[1] = Vector{ 1,1,3 };
	t.p[2] = Vector{ 0,-1,3 };
	std::vector<Triangle> vt;
	vt.push_back(t);
	//renderObject(vt);
	renderObject(scene.meshes[0]);
	std::time_t endTime;
	//Limit frame rate to reduce power consumption
	double overhead = (frameLimit * 100) - (fdt);
	if (overhead > 0) {
		Sleep(overhead);
	}
	timer::Duration deltaTime = timer_end(start,endTime);
	fdt = deltaTime.count();
	std::cout <<std::fixed<<"\r" <<1.f/(deltaTime.count()) << " FPS" << std::flush;
}