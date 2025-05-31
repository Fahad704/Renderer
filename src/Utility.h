#pragma once
#include "Typedefs.h"
template<typename T>
void swap(T& a, T& b) {
	T c;
	c = a;
	a = b;
	b = c;
}
template <typename T>
void clamp(T& num, T min_limit, T max_limit) {
	if (num < min_limit) {
		num = min_limit;
		return;
	}
	else if (num > max_limit) {
		num = max_limit;
		return;
	}
	return;
}
template<typename T>
bool isIn(T value, T lower, T higher) {
	if (value >= higher) {
		return false;
	}
	else if (value <= lower) {
		return false;
	}
	else {
		return true;
	}
}

float getMax(const float& n1, const float& n2);