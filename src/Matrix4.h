#pragma once
#include "Vector4.h"

struct Matrix4
{
	double arr[4][4];
	Matrix4() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				arr[i][j] = 0;
			}
		}
	}
	Matrix4(const Matrix4& mat4) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				arr[i][j] = mat4.arr[i][j];
			}
		}
	}
};

