#ifndef MAIN_CPP
#define MAIN_CPP
#include "Window.cpp"
#define isDown(b) input.buttons[b].isDown
#define pressed(b) (input.buttons[b].isDown && input.buttons[b].changed)
#define released(b) (!input.buttons[b].isDown && input.buttons[b].changed)

//TODO(Fahad):
/*
*	Optimizations:
*		-Implement Frustum culling
*		-Implement BVH ray tracing
*		-Implement Occlusion culling
*	Adding Features:
*		-read and display textures(Rasterizer)
*		-render according to material(Rasterizer)
*		-Make the structure API like for the renderer
*		-Add some ui
*/



bool rendMode=false;
//Back face culling
bool bfc = true;
//frame delta time
double fdt=0.06;
void handleInput(const Input& input) {
	double speed = 5.f;
	Vector move = { 0,0,0 };
	//Movement events
	if (isDown(BUTTON_RIGHT))
		move = Vector{ 10.f, 0, 0 };

	if (isDown(BUTTON_LEFT))
		move = move + Vector{ -10.f, 0, 0 };

	if (isDown(BUTTON_UP))
		move = move + Vector{ 0, 0, 10.f };

	if (isDown(BUTTON_DOWN))
		move = move + Vector{ 0, 0, -10.f };

	if (isDown(BUTTON_W))
		move = move + Vector{ 0,10.f,0 };

	if (isDown(BUTTON_S))
		move = move + Vector{ 0,-10.f,0 };
	if (pressed(BUTTON_SPACE)) {
		std::cout << "\nPosition : " << camera.position.x << " " << camera.position.y << " " << camera.position.z << "\n";
		std::cout << "Rotation : " << camera.rotation.x << " " << camera.rotation.y << " " << camera.rotation.z << "\n";
		int c = 1;
		std::cout << "Model count " << scene.instances.size() << "\n\n";
		int totalTris = 0;
		for (const Instance& instance : scene.instances)
		{
			std::cout << "Model " << c << ":\n";
			std::cout << "Triangle count : " << instance.mesh->triangles.size() << "\n";
			std::cout << "Face count : " << instance.mesh->faces.size() << "\n";
			std::cout << "Normal count : " << instance.mesh->normals.size() << "\n\n";
			totalTris += instance.mesh->triangles.size();
			c++;
		}
		std::cout << "Total triangle count :" << totalTris << '\n';
	}
	//Normalize move vector and move the Camera
	if (!(move == Vector{ 0,0,0 })) {

		move = move / length(move);
		move = move * speed;
		move = rotate(move, camera.rotation);
		camera.position = camera.position + (move * fdt);
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
	//Exporting an image
	if (pressed(BUTTON_P))
		exportToPPM("Image.ppm");
	//Backface culling toggle
	if (pressed(BUTTON_C))
		bfc = !bfc;
	//Rotate Camera Left
	if (isDown(BUTTON_Z))
		camera.rotation.y += 60.0 * fdt;
	//Rotate Camera right
	if (isDown(BUTTON_X)) {
		camera.rotation.y -= 60.0 * fdt;
	}
	//Reset camera Position and rotation
	if (pressed(BUTTON_Q)) {
		camera.rotation.y = 0;
		camera.position = { 0,0,0 };
	}
	//Slow down time
	if (isDown(MOUSE_BUTTON_LEFT)) {
		fdt = 0.0001;
	}
	if (isDown(MOUSE_BUTTON_RIGHT)) {
		scene.instances[0].transform.rotation.y += 100 * fdt;
	}
}
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
		//{LT_DIRECTIONAL,{0,0,0},{1,4,4},0.7f}
	};

	std::vector<Triangle> triangles = {};
	std::vector<Instance> instances = {};

	static Mesh model = loadOBJ("../Models/cube.obj", { 20,255,255 }, 0.5f, 1000);
	Instance ins[] = { 
		{model, {0,0,5},1,{0,0,0}},
		//{model, {-2,1,5},1,{3,0,0}},
		//{model, {1,-2,6},1,{0,8,0}},
		//{model, {3,0,7},1,{0,0,0}}
	};
	int insSize = sizeof(ins) / sizeof(Instance);
	for(int i = 0;i<insSize;i++)
		instances.push_back(ins[i]);
	scene = { spheres,triangles,instances,lights };
}
void update(const Input& input) {
	timer::Timer start = timerStart();
	
	handleInput(input);
	//Ray trace
	if (rendMode && once){
		clearScreen(0x000000);
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
		//rayTrace();
		std::cout << "\r" << "Frame rendered\n";
		std::cout << "Width : " << renderState.width << " " << renderState.height << "\n";
		std::time_t endTime;
		timer::Duration deltaTime = timer_end(start, endTime);
		fdt = deltaTime.count();
		//std::cout << std::fixed << "\r" << 1.f / (deltaTime.count()) << " FPS" << std::flush;
		once = false;
		return;
	}


	//Rasterize
	if (!rendMode) {
		clearScreen(0x646464);
		renderObject(scene.instances[0], bfc);
		//renderObject(scene.instances[1], bfc);
		//renderObject(scene.instances[2], bfc);
		//renderObject(scene.instances[3], bfc);
		once = true;
	}
	std::time_t endTime;
	//Limit frame rate to reduce power consumption
	double overhead = (frameLimit * 100) - (fdt);
	if (overhead > 0) {
		Sleep(overhead);
	}
	timer::Duration deltaTime = timer_end(start,endTime);
	fdt = deltaTime.count();
	//Display fps on console
	//std::cout <<std::fixed<<"\r" <<1.f/(deltaTime.count()) << " FPS" << std::flush;
}
#endif