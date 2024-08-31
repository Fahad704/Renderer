#ifndef WINDOW_CPP
#define WINDOW_CPP
#include "Utility.cpp"
#include <Windows.h>
global_variable bool running = true;
struct RenderState {
	int height;
	int width;
	void* memory;
	BITMAPINFO bitmapinfo;
};
void* depth;
static RenderState renderState;
static HWND window = {};

#include "Platform_common.cpp"
#include "Globals.cpp"
#include "Renderer.cpp"
#include "Resource.h"
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
	}break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hWnd, &rect);
		renderState.width = rect.right - rect.left;
		renderState.height = rect.bottom - rect.top;

		int buffer_size = renderState.width * renderState.height * sizeof(unsigned int);
		//Screen backbuffer
		if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);
		renderState.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		//Depth buffer
		if (depth)VirtualFree(depth, 0, MEM_RELEASE);
		depth = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		renderState.bitmapinfo.bmiHeader.biSize = sizeof(renderState.bitmapinfo.bmiHeader);
		renderState.bitmapinfo.bmiHeader.biWidth = renderState.width;
		renderState.bitmapinfo.bmiHeader.biHeight = renderState.height;
		renderState.bitmapinfo.bmiHeader.biPlanes = 1;
		renderState.bitmapinfo.bmiHeader.biBitCount = 32;
		renderState.bitmapinfo.bmiHeader.biCompression = BI_RGB;
		canvas = { double(renderState.width), double(renderState.height)};
		double aspectratio = double(renderState.width) / double(renderState.height);
		vpWidth = 1;
		vpHeight = aspectratio;
	}break;
	default: {
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	}
	return result;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	//Random seed
	srand(time(NULL));
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

	//Create Window 
	window = CreateWindow(window_class.lpszClassName, L"Renderer!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 720, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);
	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
	AllocConsole();
	std::freopen("CONOUT$", "w", stdout);
	
	Input input = {};
	init();
	while (running) {

		//Message loop
		MSG message;
		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}
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
					process_messages(BUTTON_DOWN, VK_DOWN);
					process_messages(BUTTON_LEFT, VK_LEFT);
					process_messages(BUTTON_RIGHT, VK_RIGHT);
					process_messages(BUTTON_SPACE, VK_SPACE);
					process_messages(BUTTON_R, 'R');
					process_messages(BUTTON_W, 'W');
					process_messages(BUTTON_A, 'A');
					process_messages(BUTTON_V, 'V');
					process_messages(BUTTON_T, 'T');
					process_messages(BUTTON_B, 'B');
					process_messages(BUTTON_S, 'S');
					process_messages(BUTTON_P, 'P');
					process_messages(BUTTON_D, 'D');
					process_messages(BUTTON_C, 'C');
					process_messages(BUTTON_Z, 'Z');
					process_messages(BUTTON_Q, 'Q');
					process_messages(BUTTON_X, 'X');
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

		//Game Update Loop
		update(&input);
			

		//Render
		StretchDIBits(hdc, 0, renderState.height-1, renderState.width, -renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
	}
}
#endif