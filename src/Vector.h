#ifndef VECTOR_H
#define VECTOR_H
struct Vector {
	double x;
	double y;
	double z;
	Vector(double x = 0, double y = 0, double z = 0) {
		Vector::x = x;
		Vector::y = y;
		Vector::z = z;
	}
	friend Vector operator+(const Vector&,const Vector&);
	friend Vector operator+(const Vector&,const double&);
	friend Vector operator*(const Vector&,const Vector&);
	friend Vector operator*(const Vector&,const double&);
	friend Vector operator/(const Vector&,const Vector&);
	friend Vector operator/(const Vector&,const double&);
	friend Vector operator-(const Vector&,const Vector&);
	friend Vector operator-(const Vector&,const double&);
	friend Vector operator-(const Vector&);
	friend bool operator==(const Vector&, const Vector&);
	friend bool operator!=(const Vector& vec, const Vector&);
	friend Vector operator*(const double, const Vector&);
	friend Vector operator/(const double, const Vector&);
	friend Vector operator-(const Vector&, const Vector&);
	friend Vector cross(const Vector&, const Vector&);
	friend double dot(const Vector&,const Vector&);
	friend double length(const Vector&);
};
#endif