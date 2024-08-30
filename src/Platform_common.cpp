#ifndef PLATFORM_COMMON_CPP
#define PLATFORM_COMMON_CPP
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
	BUTTON_C,
	BUTTON_V,
	BUTTON_T,
	BUTTON_P,
	BUTTON_SPACE,
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