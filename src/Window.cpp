#ifndef WINDOW_CPP
#define WINDOW_CPP
#define _CRT_SECURE_NO_WARNINGS
/*
* ------------------------------------------------------------------
* This file handles all the windowing and windows api specific tasks
* ------------------------------------------------------------------
*/
#include <Windows.h>
struct RenderState {
	int height;
	int width;
	void* memory;
	BITMAPINFO bitmapinfo;
};
void* depth;
static RenderState renderState;
static HWND window = {};

#include "Utility.cpp"
#include "Platform_common.h"
global_variable bool running = true;
void init();
void update(const Input&);
#include "Globals.h"
#include "Renderer.cpp"
#include "Resource.h"
#define isDown(b) input.buttons[b].isDown
#define pressed(b) (input.buttons[b].isDown && input.buttons[b].changed)
#define released(b) (!input.buttons[b].isDown && input.buttons[b].changed)
#include "Main.cpp"
void clearScreen(u32);
LRESULT CALLBACK window_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;
		if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);
		if (depth)VirtualFree(depth, 0, MEM_RELEASE);
		FreeConsole();
		std::fclose(stdout);
		DestroyWindow(hWnd);
		return DefWindowProc(window, uMsg, wParam, lParam);
	}break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hWnd, &rect);
		renderState.width = rect.right - rect.left;
		renderState.height = rect.bottom - rect.top;

		int bufferSize = renderState.width * renderState.height * sizeof(unsigned int);
		int dbufSize = renderState.width * renderState.height * sizeof(float);
		//Screen backbuffer
		if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);
		renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		//Depth buffer
		if (depth)VirtualFree(depth, 0, MEM_RELEASE);
		depth = VirtualAlloc(0, dbufSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		renderState.bitmapinfo.bmiHeader.biSize = sizeof(renderState.bitmapinfo.bmiHeader);
		renderState.bitmapinfo.bmiHeader.biWidth = renderState.width;
		renderState.bitmapinfo.bmiHeader.biHeight = renderState.height;
		renderState.bitmapinfo.bmiHeader.biPlanes = 1;
		renderState.bitmapinfo.bmiHeader.biBitCount = 32;
		renderState.bitmapinfo.bmiHeader.biCompression = BI_RGB;
		canvas = { float(renderState.width), float(renderState.height)};
		float aspectratio = float(renderState.width) / float(renderState.height);
		change = true;
		vpWidth = aspectratio;
		vpHeight = 1;
	}break;
	default: {
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	}
	return result;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	//Random seed
	srand(u32(time(NULL)));
	//Create Window Class
	WNDCLASS window_class = {};
	window_class.lpfnWndProc = window_callback;
	window_class.lpszClassName = L"Window Class";
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	HCURSOR hCursorArrow = LoadCursor(NULL, IDC_ARROW);
	window_class.hCursor = hCursorArrow;
	window_class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	
	//Register Window Class 
	RegisterClass(&window_class);

	//Create console
	AllocConsole();
	std::freopen("CONOUT$", "w", stdout);

	//Create Window 
	window = CreateWindow(window_class.lpszClassName, L"Renderer!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 720, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);
	ShowCursor(false);
	SetCursorPos(0, 0);
	SetWindowPos(window, NULL,800,100, renderState.width, renderState.height, NULL);
	Input input = {};

	init();
	while (running) {

		MSG message;
		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}
		//Message loop
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {
				u32 vk_code = (u32)message.wParam;
				bool isDown = ((message.lParam & (1 << 31)) == 0);
#define process_messages(b,vk)\
case vk:{\
input.buttons[b].changed = (isDown != input.buttons[b].isDown);\
input.buttons[b].isDown = isDown;\
}break;
				switch (vk_code) {
					process_messages(BUTTON_UP,VK_UP)
					process_messages(BUTTON_DOWN, VK_DOWN)
					process_messages(BUTTON_LEFT, VK_LEFT)
					process_messages(BUTTON_RIGHT, VK_RIGHT)
					process_messages(BUTTON_SPACE, VK_SPACE)
					process_messages(BUTTON_ESC, VK_ESCAPE)
					process_messages(BUTTON_SHIFT, VK_SHIFT)
					process_messages(BUTTON_CTRL, VK_CONTROL)
					process_messages(BUTTON_A, 'A')
					process_messages(BUTTON_B, 'B')
					process_messages(BUTTON_C, 'C')
					process_messages(BUTTON_D, 'D')
					process_messages(BUTTON_F, 'F')
					process_messages(BUTTON_G, 'G')
					process_messages(BUTTON_L, 'L')
					process_messages(BUTTON_M, 'M')
					process_messages(BUTTON_N, 'N')
					process_messages(BUTTON_P, 'P')
					process_messages(BUTTON_Q, 'Q')
					process_messages(BUTTON_R, 'R')
					process_messages(BUTTON_S, 'S')
					process_messages(BUTTON_T, 'T')
					process_messages(BUTTON_V, 'V')
					process_messages(BUTTON_W, 'W')
					process_messages(BUTTON_X, 'X')
					process_messages(BUTTON_Z, 'Z')
				}
			}break;
			case WM_LBUTTONDOWN: {
				input.buttons[MOUSE_BUTTON_LEFT].changed = !(input.buttons[MOUSE_BUTTON_LEFT].isDown);
				input.buttons[MOUSE_BUTTON_LEFT].isDown = true;

			}break;
			case WM_LBUTTONUP: {
				input.buttons[MOUSE_BUTTON_LEFT].changed = input.buttons[MOUSE_BUTTON_LEFT].isDown;
				input.buttons[MOUSE_BUTTON_LEFT].isDown = false;

			}break;
			case WM_RBUTTONDOWN: {
				input.buttons[MOUSE_BUTTON_RIGHT].changed = !(input.buttons[MOUSE_BUTTON_RIGHT].isDown);
				input.buttons[MOUSE_BUTTON_RIGHT].isDown = true;

			}break;
			case WM_RBUTTONUP: {
				input.buttons[MOUSE_BUTTON_RIGHT].changed = input.buttons[MOUSE_BUTTON_RIGHT].isDown;
				input.buttons[MOUSE_BUTTON_RIGHT].isDown = false;

			}break;
			default: {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			}
		}
		//Update Loop
		update(input);
			

		//Draw buffer
		//TODO(Fahad) find a better way to display buffer to the screen
		if(sceneSettings.renderMode == RenderMode::RM_DEPTH){
			for(int y=0;y<renderState.height;y++){
				for(int x=0;x<renderState.width;x++){
					float* value = (((float *)depth) + x + (y * renderState.width));
					if(!value){
						std::cout<<"Value is NULLPTR\n";
					}
					Colour color = {(unsigned char)((*value)*255),(unsigned char)((*value)*255),(unsigned char)((*value)*255)};
					Renderer::putPixelD(x,y,color);
				}
			}
		}
		StretchDIBits(hdc, 0, renderState.height-1, renderState.width, -renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
	}
	ReleaseDC(window, hdc);
	return 0;
}
#endif