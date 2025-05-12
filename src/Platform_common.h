#ifndef PLATFORM_COMMON_H
#define PLATFORM_COMMON_H
//this says platform common but there is only one platform lol
enum Buttons {
	BUTTON_SHIFT,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_RIGHT,
	BUTTON_LEFT,
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	BUTTON_SPACE,
	BUTTON_ESC,
	BUTTON_CTRL,
	BUTTON_A,
	BUTTON_B,
	BUTTON_C,
	BUTTON_D,
	BUTTON_F,
	BUTTON_G,
	BUTTON_L,
	BUTTON_M,
	BUTTON_N,
	BUTTON_P,
	BUTTON_Q,
	BUTTON_R,
	BUTTON_S,
	BUTTON_T,
	BUTTON_V,
	BUTTON_W,
	BUTTON_X,
	BUTTON_Z,
	BUTTON_COUNT
};

struct Button_state {
	bool isDown;
	bool changed;
};
struct Input {
	Button_state buttons[BUTTON_COUNT];
};
#endif