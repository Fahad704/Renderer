#ifndef MAIN_CPP
#define MAIN_CPP
#include "Renderer.cpp"


//TODO(Fahad):
/*
*	Optimizations:
*		-Implement Frustum culling(Proper one)
*		-Implement BVH ray tracing
*		-Implement Occlusion culling
*	Adding Features:
*		-interpolated normals(Rasterizer)
*		-read and display textures(Rasterizer)
*		-render according to material(Rasterizer)
*		-Add some ui
*	Code Refactoring:
*		-Make the structure API like for the renderer
* 
*/
//frame delta time
double fdt=0.06;
void handleInput(const Input& input) {
	double speed = 5.f;
	Vector move = { 0,0,0 };
	//Movement events
	if (isDown(BUTTON_ESC)) {
		running = false;
	}
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
		change = true;
		move = move / length(move);
		move = move * speed;
		move = rotate(move, camera.rotation);
		camera.position = camera.position + (move * fdt);
	}
	//Show triangles of the mesh
	if (pressed(BUTTON_T)) {
		if (sceneSettings.debugState != DebugState::DS_TRIANGLE)std::cout << "\nDebug state set to wireframe triangle\n";
		sceneSettings.debugState = DebugState::DS_TRIANGLE;
		change = true;
	}
	//Show bounding box of the mesh
	if (pressed(BUTTON_B)) {
		if (sceneSettings.debugState != DebugState::DS_BOUNDING_BOX)std::cout << "\nDebug state set to bounding box\n";
		sceneSettings.debugState = DebugState::DS_BOUNDING_BOX;
		change = true;
	}
	//Turn off Debug view
	if (pressed(BUTTON_V)) {
		if(sceneSettings.debugState != DebugState::DS_OFF)std::cout << "\nVisual debugging off\n";
		sceneSettings.debugState = DebugState::DS_OFF;
		change = true;
	}
	//Change ray tracing to rasterization and vise versa
	if (pressed(BUTTON_R)) {
		rendMode = !rendMode;
		if (rendMode) {
			std::cout << "\nRay tracing turned on\n";
		}else{
			std::cout << "\nRay tracing turned off\n";
		}
		change = true;
	}
	//Exporting an image
	if (pressed(BUTTON_P))
		Renderer::exportToPPM("Image.ppm");
	//Backface culling toggle
	if (pressed(BUTTON_C)) {
		sceneSettings.bfc = !sceneSettings.bfc;
		if (sceneSettings.bfc)
			std::cout << "\nBackface culling turned on\n";
		else
			std::cout << "\nBackface culling turned off\n";
		change = true;
	}
	//Rotate Camera Left
	if (isDown(BUTTON_Z)) {
		camera.rotation.y += 60.0 * fdt;
		change = true;
	}
	//Rotate Camera right
	if (isDown(BUTTON_X)) {
		camera.rotation.y -= 60.0 * fdt;
		change = true;
	}
	//Reset camera Position and rotation
	if (pressed(BUTTON_Q)) {
		camera.rotation = { 0,0,0 };
		camera.position = { 0,0,0 };
		change = true;
	}
	//Slow down time
	if (isDown(MOUSE_BUTTON_LEFT)) {
		fdt = 0.001;
	}
	if (isDown(MOUSE_BUTTON_RIGHT)) {
		Transform tf = { {0,0,0},1,{0,100 * fdt,0} };
		scene.instances[0].applyTransform(tf);
		change = true;
	}
	if (pressed(BUTTON_A)) {
		//Anti aliasing
		sceneSettings.antiAliasing = !sceneSettings.antiAliasing;
		if (sceneSettings.antiAliasing) {
			std::cout << "\nAnti aliasing turned on.\n";
		}
		else {
			std::cout << "\nAnti aliasing turned off.\n";
		}
		change = true;
	}
	if (isDown(BUTTON_N)) {
		camera.rotation.x += 60.0 * fdt;
		change = true;
	}
	if (isDown(BUTTON_M)) {
		camera.rotation.x -= 60.0 * fdt;
		change = true;
	}
}
void init() {
	//Spheres (currently only rendered in ray tracing)
	std::vector<Sphere> spheres = {};

	std::vector<Light> lights = {
		//--Type----------Pos----Dir--intensity//
		{LT_AMBIENT    ,{0,0,0},{0,0,0},0.2f},
		{LT_POINT      ,{2,1,0},{0,0,0},0.6f},
		{LT_DIRECTIONAL,{0,0,0},{1,-4,4},0.2f},
		{LT_DIRECTIONAL,{0,0,0},{-1,-1,4},0.2f},
	};

	std::vector<Triangle> triangles = {};
	std::vector<Instance> instances = {};

	static Mesh model = Renderer::loadOBJ("../Models/King.obj", { 255,255,255 }, 0.f,1000);
	Instance ins[] = {
		{model, {0,-0.8f,3},1,{0,0,0}},
		//{model, {-2,1,7},1,{3,0,0}},
		//{model, {1,-2,8},1,{0,8,0}},
		//{model, {3,0,9},1,{0,0,0}}
	};
	int insSize = sizeof(ins) / sizeof(Instance);
	for(int i = 0;i<insSize;i++)
		instances.push_back(ins[i]);
	scene = { spheres,triangles,instances,lights };
}
void update(const Input& input) {
	//Start counting frame time
	timerStart();
	handleInput(input);
	//Ray trace
	if (rendMode && change) {
		Renderer::clearScreen(0x000000);
		//Ray tracing with 12 threads
		size_t threadCount = 12;
		std::vector<std::thread> tObjs(threadCount);
		for (size_t i = 0; i < threadCount; i++)
		{
			tObjs[i] = std::thread(Renderer::rayTraceThr, i, threadCount);
		}
		for (size_t i = 0; i < tObjs.size(); i++) {
			tObjs[i].join();
		}
		if (sceneSettings.antiAliasing && (sceneSettings.debugState != DebugState::DS_TRIANGLE)) {
			Renderer::FXAA();
		}
		change = false;
	}

	//Rasterize
	if (!rendMode && change) {
		Renderer::renderScene();
		change = false;
	}
	
	//Limit frame rate to reduce power consumption
	if (fdt < frameLimit) {
		std::this_thread::sleep_for(std::chrono::milliseconds(int((frameLimit) - fdt)));
	}
	std::chrono::duration<double> deltaTime = timerEnd(tstart);
	fdt = deltaTime.count();

	//Display fps on console
	std::cout << std::fixed<<"\r" << (double)1 / fdt << " FPS - " << "Tri Rendering: " << sceneSettings.triSeenCount;
}
#endif