#pragma once
struct Vector {
	float x;
	float y;
	float z;
	Vector(float x = 0, float y = 0, float z = 0) {
		Vector::x = x;
		Vector::y = y;
		Vector::z = z;
	}
	friend Vector operator+(const Vector&, const Vector&);
	friend Vector operator+(const Vector&, const float&);
	friend Vector operator*(const Vector&, const Vector&);
	friend Vector operator*(const Vector&, const float&);
	friend Vector operator/(const Vector&, const Vector&);
	friend Vector operator/(const Vector&, const float&);
	friend Vector operator-(const Vector&, const Vector&);
	friend Vector operator-(const Vector&, const float&);
	friend Vector operator-(const Vector&);
	friend bool operator==(const Vector&, const Vector&);
	friend bool operator!=(const Vector& vec, const Vector&);
	friend Vector operator*(const float, const Vector&);
	friend Vector operator/(const float, const Vector&);
	friend Vector operator-(const Vector&, const Vector&);
	friend Vector cross(const Vector&, const Vector&);
	friend float dot(const Vector&, const Vector&);
	friend float length(const Vector&);
};