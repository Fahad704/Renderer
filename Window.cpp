#pragma once
#include "Utility.cpp"
#include <Windows.h>
global_variable bool running = true;
struct Render_State {
	int height;
	int width;
	void* memory;
	BITMAPINFO bitmapinfo;
};

static Render_State render_state;
static HWND window = {};

#include "Platform_common.cpp"
#include "Globals.cpp"
#include "Renderer.cpp"
#include "Resource.h"
#include "Main.cpp"
void clearScreen(u32);
LRESULT window_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;
		if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
		FreeConsole();
		std::fclose(stdout);
		DestroyWindow(hWnd);
	}break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hWnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;

		int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);

		if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
		render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		render_state.bitmapinfo.bmiHeader.biSize = sizeof(render_state.bitmapinfo.bmiHeader);
		render_state.bitmapinfo.bmiHeader.biWidth = render_state.width;
		render_state.bitmapinfo.bmiHeader.biHeight = render_state.height;
		render_state.bitmapinfo.bmiHeader.biPlanes = 1;
		render_state.bitmapinfo.bmiHeader.biBitCount = 32;
		render_state.bitmapinfo.bmiHeader.biCompression = BI_RGB;
		canvas = { double(render_state.width), double(render_state.height)};
		double aspectratio = double(render_state.width) / double(render_state.height);
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
	//random seed
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
	AllocConsole();
	std::freopen("CONOUT$", "w", stdout);
	
	Input input = {};
	init();
	clearScreen(0x000000);
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
					process_messages(BUTTON_R, 'R');
					process_messages(BUTTON_W, 'W');
					process_messages(BUTTON_A, 'A');
					process_messages(BUTTON_V, 'V');
					process_messages(BUTTON_T, 'T');
					process_messages(BUTTON_B, 'B');
					process_messages(BUTTON_S, 'S');
					process_messages(BUTTON_D, 'D');
				}
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
		StretchDIBits(hdc, 0, render_state.height-1, render_state.width, -render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
	}
}