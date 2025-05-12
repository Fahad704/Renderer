#pragma once
#include "Vector.h"
struct Vector4
{
	double x, y, z, w;
	Vector4(double x = 0, double y = 0, double z = 0, double w = 0) {
		Vector4::x = x;
		Vector4::y = y;
		Vector4::z = z;
		Vector4::w = w;
	}
	Vector4(const Vector& vec3) {
		Vector4::x = vec3.x;
		Vector4::y = vec3.y;
		Vector4::z = vec3.z;
		Vector4::w = 0;
	}

};
