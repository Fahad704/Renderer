#pragma once
enum Buttons {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_R,
	BUTTON_G,
	BUTTON_B,
	BUTTON_RIGHT,
	BUTTON_LEFT,
	BUTTON_W,
	BUTTON_A,
	BUTTON_S,
	BUTTON_D,
	BUTTON_V,
	BUTTON_T,
	BUTTON_COUNT
};

struct Button_state {
	bool isDown;
	bool changed;
};
struct Input {
	Button_state buttons[BUTTON_COUNT];
};