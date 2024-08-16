#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <sstream>
#include <unordered_map>
#include <typeinfo>
#include <glad/glad.h>
#include <iomanip>
#include <thread> 
#include <time.h>
#include "Window.cpp"
#include "Vector.cpp"
#define infinity 2000000000

typedef unsigned int u32;
typedef unsigned char u8;
#define internal static
#define global_variable static
namespace timer {
	typedef std::chrono::duration<double> Duration;
	typedef std::chrono::time_point<std::chrono::system_clock> Timer;
};
timer::Timer timerStart() {
	timer::Timer start = std::chrono::system_clock::now();
	return start;
}
std::chrono::duration<double> timer_end(timer::Timer start,std::time_t& end_time) {
	timer::Timer end = std::chrono::system_clock::now();
	end_time = std::chrono::system_clock::to_time_t(end);
	return (end - start);
}
enum LightType {
	LT_POINT,
	LT_DIRECTIONAL,
	LT_AMBIENT
};
enum class DebugState {
	DS_OFF,
	DS_BOUNDING_BOX,
	DS_TRIANGLE
};
template<typename T>
void swap(T& a, T& b) {
	T c;
	c = a;
	a = b;
	b = c;
}
void clamp(double& num, double min_limit, double max_limit) {
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
void clamp(u32& num, u32 min_limit, u32 max_limit) {
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
struct Colour {
	u8 R;
	u8 G;
	u8 B;
public:
	Colour() {
		R = 0;
		G = 0;
		B = 0;
	}
	Colour(unsigned char R, unsigned char G, unsigned char B) {
		this->R = R;
		this->G = G;
		this->B = B;
	}

	bool operator==(Colour& op) {
		return (this->R == op.R && this->G == op.G && this->B == op.B);
	}
	Colour operator*(const double num) {
		Vector newcol = { double(this->R * num),double(this->G * num),double(this->B * num) };
		if (newcol.x > 255) {
			newcol.x = 255;
		}
		if (newcol.y > 255) {
			newcol.y = 255;
		}
		if (newcol.z > 255) {
			newcol.z = 255;
		}
		return { u8(newcol.x),u8(newcol.y),u8(newcol.z) };
	}
	Colour operator+(const Colour& col) {
		Vector newcol = { double(this->R + col.R),double(this->G + col.G),double(this->B + col.B) };
		if (newcol.x > 255) {
			newcol.x = 255;
		}
		if (newcol.y > 255) {
			newcol.y = 255;
		}
		if (newcol.z > 255) {
			newcol.z = 255;
		}
		return { u8(newcol.x),u8(newcol.y),u8(newcol.z) };
	}
};
internal Colour hexToRGB(u32 hex) {
	Colour color;
	color.R = u8((hex >> 16) & 0xff);
	color.G = u8((hex >> 8) & 0xff);
	color.B = u8(hex & 0xff);
	return color;
}
u32 rgbtoHex(Colour RGB) {
	return u32((RGB.R << 16) | (RGB.G << 8) | RGB.B);
}
enum class Type
{
	ST_BASE=0,
	ST_SPHERE,
	ST_TRIANGLE
};
struct Object {
	Colour color;
	double specular;
	double reflectiveness;
	virtual Type getType()
	{
		return Type::ST_BASE;
	}
};
struct Sphere : Object{
	double radius;
	Vector center;
	
	Sphere(Vector center = { 0,0,0 }, double radius = NULL, Colour color = {0,0,0},double specular = -1,double reflectiveness = 0) {
		this->center = center;
		this->radius = radius;
		this->color = color;
		this->specular = specular;
		this->reflectiveness = reflectiveness;
	}
	virtual Type getType()
	{
		return Type::ST_SPHERE;
	}
};
struct Triangle : Object {
	Vector p[3];
	Vector normal;
	Triangle() {
		p[0] = {};
		p[1] = {};
		p[2] = {};
		color = { 255,0,0 };
		reflectiveness = 0;
		specular = -1;
	}
	Triangle(const Vector p[3], Vector normal = {0,0,0}, Colour color = { 0,0,0 }, double specular = -1, double reflectiveness = 0) {
		this->p[0] = p[0];
		this->p[1] = p[1];
		this->p[2] = p[2];
		this->color = color;
		this->specular = specular;
		this->reflectiveness = reflectiveness;
	}
	virtual Type getType()
	{
		return Type::ST_TRIANGLE;
	}
	Vector getNormal()
	{
		//anti clockwise
		if (normal == Vector{0, 0, 0})
		{
			normal = cross((p[1] - p[0]), (p[2] - p[0]));
			return normal;
		}
		return normal;
	}
};
struct Texture
{
	double u;
	double v;
	double w;
};
struct Index
{
	int vert;
	int text;
	int norm;
};
struct Face
{
	Index index[3];
};
struct Box {
	Vector highest;
	Vector lowest;
	friend bool operator==(const Box& box2,const Box& box) {
		return ((box.lowest == box2.lowest) && (box.highest == box2.highest));
	}
};
struct Mesh {
	std::vector<Vector> vertexes;
	std::vector<Vector> normals;
	std::vector<Texture> texture;
	std::vector<Face> faces;
	std::vector<Triangle> triangles;
	Box boundingBox;
	double reflectiveness;
	double specular;
	Mesh() {
		vertexes = {};
		normals = {};
		texture = {};
		faces = {};
		triangles = {};
		boundingBox = {};
		m_pos = {};
		reflectiveness = 0;
		specular = -1;
	}
	Mesh(std::vector<Vector> vertex, std::vector<Vector> normal = {}, std::vector<Texture> text = {}, std::vector<Face> face = {}, std::vector<Triangle> triangle = {},Vector pos = {0,0,0},double reflectiveness = 0.f, double specular = -1)
	{
		vertexes = vertex;
		normals = normal;
		texture = text;
		faces = face;
		triangles = triangle;
		m_pos = pos;
		if (pos != Vector{0, 0, 0}) {
			setPos(pos);
		}
		this->reflectiveness = reflectiveness;
		this->specular = specular;
	}
	std::vector<Triangle> initTriangles(Colour color)
	{
		Vector lowest, highest;
		int count=0;
		//if (!triangles.size()) {
			for (const Face& face : faces) {
				Triangle triangle;
				Vector v1, v2, v3;
				v1 = vertexes.at(face.index[0].vert-1);
				v2 = vertexes.at(face.index[1].vert-1);
				v3 = vertexes.at(face.index[2].vert-1);
				triangle.reflectiveness = reflectiveness;
				triangle.specular = specular;
				triangle.p[0] = v1 + m_pos;
				triangle.p[1] = v2 + m_pos;
				triangle.p[2] = v3 + m_pos;
				triangle.color = color;
				triangle.normal = face.index[0].norm;
				triangles.push_back(triangle);
			}
			for (const Vector& vertex : vertexes) {
				if (count == 0) {
					lowest = vertex;
					highest = vertex;
				}
				if (vertex.x < lowest.x)lowest.x = vertex.x;
				if (vertex.y < lowest.y)lowest.y = vertex.y;
				if (vertex.z < lowest.z)lowest.z = vertex.z;
				if (vertex.x > highest.x)highest.x = vertex.x;
				if (vertex.y > highest.y)highest.y = vertex.y;
				if (vertex.z > highest.z)highest.z = vertex.z;
				count++;
			}
		//}
		 //else{
			//return triangles;
		//}
		boundingBox.lowest = lowest + m_pos;
		boundingBox.highest = highest + m_pos;
		return triangles;
	}
	void setPos(Vector pos,Colour color = {}) {
		m_pos = pos;
		initTriangles(color);
	}
private:
	Vector m_pos;
};
struct Light {
	LightType type;
	Vector pos;
	Vector direction;
	double intensity;
};
template<typename T>
internal bool isIn(T value,T lower, T higher) {
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
struct Scene {
	std::vector<Sphere> spheres;
	std::vector<Triangle> triangles;
	std::vector<Mesh> meshes;
	std::vector<Light> lights;
};