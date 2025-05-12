#ifndef MAIN_CPP
#define MAIN_CPP

#include "Renderer.cpp"

//TODO(Fahad):
/*
*	Optimizations:
*		-Implement BVH ray tracing
*		-Implement proper Occlusion culling
*	Adding Features:
*		-Add better logging
*		-Add matrix transformation for renderer
*		-interpolated normals(Rasterizer)
*		-read and display textures(Rasterizer)
*		-render according to material(Rasterizer)
*		-Add some ui
*	Code Refactoring:
*		-Make better file structure
*		-Abstract Windowing into classes
*		-Make the structure API like for the renderer
*
*/
//frame delta time
double fdt = 0.06;
float totalFrameTime = 0.f;
int frameCount = 0;
const float defSpeed = 2.f;
float speed = defSpeed;
const float boostSpeed = (5 * defSpeed);
void handleInput(const Input& input) {
	const float sensitivity = 1.2f;
	Vector mouseDiff = sceneSettings.lockMouse ? getMouseDiff() : 0;
	mouseDiff = mouseDiff * sensitivity;
	Vector velocity = { 0.f,0.f,0.f };
	if (isDown(BUTTON_ESC)) {
		running = false;
	}
	//Movement events
	if (isDown(BUTTON_W))
		velocity = velocity + Vector{ 0,0,1.f };
	if (isDown(BUTTON_A))
		velocity = velocity + Vector{ -1.f,0,0 };

	if (isDown(BUTTON_S))
		velocity = velocity + Vector{ 0,0,-1.f };
	if (isDown(BUTTON_D)) {
		velocity = velocity + Vector{ 1.f,0,0 };
	}
	if (isDown(BUTTON_CTRL))
		velocity = velocity + Vector{ 0,-1.f,0 };
	if (isDown(BUTTON_SPACE)) {
		velocity = velocity + Vector{ 0,1.f,0 };
	}

	if (isDown(BUTTON_SHIFT)) {
		speed += 10 * fdt;
		if (speed > boostSpeed) {
			speed = boostSpeed;
		}
	}
	else {
		speed -= 10 * fdt;
		if (speed < defSpeed) {
			speed = defSpeed;
		}
		speed = defSpeed;
	}

	//Normalize move vector and move the Camera
	if (!(velocity == Vector{ 0,0,0 })) {
		change = true;
		velocity = velocity / length(velocity);
		velocity = velocity * speed;
		velocity = rotate(velocity, { 0,camera.rotation.y,0 });
		camera.position = camera.position + (velocity * fdt);
	}
	if (pressed(BUTTON_L)) {
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
		if (sceneSettings.debugState != DebugState::DS_OFF)std::cout << "\nVisual debugging off\n";
		sceneSettings.debugState = DebugState::DS_OFF;
		change = true;
	}
	//Change ray tracing to rasterization and vise versa
	if (pressed(BUTTON_R)) {
		rayTraceMode = !rayTraceMode;
		if (rayTraceMode) {
			std::cout << "\nRay tracing turned on\n";
		}
		else {
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
	//Reset camera Position and rotation
	if (pressed(BUTTON_Q)) {
		camera.rotation = { 0,0,0 };
		camera.position = { 0,0,0 };
		change = true;
	}
	//Slow down time
	fdt = 0.06;
	if (isDown(MOUSE_BUTTON_LEFT)) {
		fdt = 0.001;
	}
	if (isDown(MOUSE_BUTTON_RIGHT)) {
		Transform tf = { {0,0,0},1,{0,float(100 * fdt),0} };
		scene.instances[0].applyTransform(tf);
		change = true;
	}
	if (pressed(BUTTON_F)) {
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
	if (pressed(BUTTON_G)) {
		ShowCursor(sceneSettings.lockMouse);
		sceneSettings.lockMouse = !sceneSettings.lockMouse;
	}
	if (pressed(BUTTON_M)) {
		sceneSettings.renderMode = ((sceneSettings.renderMode == RenderMode::RM_DEPTH) ? RenderMode::RM_COLOR : RenderMode::RM_DEPTH);
	}
	//Move according to mouse difference
	if (mouseDiff != Vector{ 0,0,0 }) {
		camera.rotation.y -= mouseDiff.x * fdt;
		camera.rotation.x += mouseDiff.y * fdt;
		change = true;
	}
}
void init() {
	//Spheres
	std::vector<Sphere> spheres = {
		{{0,0,-3},1.f,{255,0,0},100,0.4f },
		{{-1,0,-4},1.f,{0,255,0},100,0.4f },
		{{1,0,-4},1.f,{0,0,255},100,0.4f },
		{{2,1,0},0.1f,{0,0,255},100,0.4f }
	};

	std::vector<Light> lights = {
		//--Type----------Pos----Dir--intensity//
		{LT_AMBIENT    ,{0,0,0},{0,0,0},0.2f},
		{LT_POINT      ,{2,1,0},{0,0,0},0.6f},
		{LT_DIRECTIONAL,{0,0,0},{1,-4,4},0.2f},
		{LT_DIRECTIONAL,{0,0,0},{-1,-1,4},0.2f},
	};

	std::vector<Triangle> triangles = {};
	std::vector<Instance> instances = {};

	static Mesh model = Renderer::loadOBJ("res/Models/cube.obj", { 255,255,255 }, 0.f, -1.f);
	instances = {
		{model, {0,0,5},1.f,{0,0,0}},
	};

	scene = { spheres,triangles,instances,lights };
}
void update(const Input& input) {
	//Start counting frame time
	Timer timer;
	handleInput(input);
	if (rayTraceMode && change) {
		//Ray trace
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
	else if (change) {
		//Rasterize
		Renderer::renderScene();
	}
	//Limit frame rate to reduce power consumption
	timer.Stop();
	if (timer.dtms < frameLimit) {
		Sleep(frameLimit - timer.dtms);
		timer.dtms += (frameLimit - timer.dtms);
	}
	//FPS count
	totalFrameTime += (timer.dtms * 0.001);
	frameCount++;
	//std::cout << "CUR : " << 1 / (timer.dtms * 0.001) << " AVG : " << (1 / (totalFrameTime / frameCount)) << " FRAMETIME: " << (timer.dtms) << "ms\n";
	timer.dtms = 0;
}
#endif
