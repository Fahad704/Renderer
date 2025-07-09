#pragma once
#include "Colour.h"
#include "Vector.h"
#include <vector>
struct HitData {
	float intersection = INFINITY_V;
	Colour color = Colour{0,0,0};
	Vector normal = {0,0,0};
	float specular = -1.f;
	float reflectiveness = 0.f;
};
struct Object {
	Colour color = {0,0,0};
	float specular = -1.f;
	float reflectiveness = 0.f;
};
struct Sphere{
	Vector center;
	float radius;
	Colour color;
	float specular = -1.f;
	float reflectiveness = 0.f;
	/*Sphere(Vector center = { 0,0,0 }, float radius = 1.f, Colour color = { 0,0,0 }, float specular = -1, float reflectiveness = 0) {
		this->center = center;
		this->radius = radius;
		this->color = color;
		this->specular = specular;
		this->reflectiveness = reflectiveness;
	}*/
	bool operator==(const Sphere& sphere) const {
		return ((sphere.color == this->color) && (sphere.specular == this->specular) && (sphere.reflectiveness == this->reflectiveness));
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
	Triangle(const Vector p[3], Vector normal = { 0,0,0 }, Colour color = { 0,0,0 }, float specular = -1, float reflectiveness = 0) {
		this->p[0] = p[0];
		this->p[1] = p[1];
		this->p[2] = p[2];
		this->color = color;
		this->specular = specular;
		this->reflectiveness = reflectiveness;
	}
	Vector getNormal()
	{
		//anti clockwise
		if (normal == Vector{ 0, 0, 0 })
		{
			normal = cross((p[1] - p[0]), (p[2] - p[0]));
			return normal;
		}
		return normal;
	}
	Vector getCentroid() {
		return ((p[0] + p[1] + p[2]) / 3);
	}
};
struct Plane {
	Vector normal;
	float offset;
};
struct Texture
{
	float u;
	float v;
	float w;
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
	friend bool operator==(const Box& box2, const Box& box) {
		return ((box.lowest == box2.lowest) && (box.highest == box2.highest));
	}
};
struct Mesh {
	std::vector<Vector> vertices;
	std::vector<Vector> normals;
	std::vector<Texture> texture;
	std::vector<Face> faces;
	std::vector<Triangle> triangles;
	Box boundingBox;
	Colour color;
	float reflectiveness;
	float specular;
	Mesh() {
		vertices = {};
		normals = {};
		texture = {};
		faces = {};
		triangles = {};
		boundingBox = {};
		reflectiveness = 0;
		specular = -1;
	}
	Mesh(std::vector<Vector> vertex, std::vector<Vector> normal = {}, std::vector<Texture> text = {}, std::vector<Face> face = {}, std::vector<Triangle> triangle = {}, Colour color = { 0,0,0 }, float reflectiveness = 0.f, float specular = -1)
	{
		vertices = vertex;
		normals = normal;
		texture = text;
		faces = face;
		triangles = triangle;
		this->color = color;
		this->reflectiveness = reflectiveness;
		this->specular = specular;
	}
	void initTriangles()
	{
		Vector lowest, highest;
		int count = 0;
		if (triangles.size()) {
			return;
		}
		for (const Face& face : faces) {
			triangles.reserve(faces.size());
			Triangle triangle;
			Vector v1, v2, v3;
			v1 = vertices.at(face.index[0].vert - 1);
			v2 = vertices.at(face.index[1].vert - 1);
			v3 = vertices.at(face.index[2].vert - 1);
			triangle.reflectiveness = reflectiveness;
			triangle.specular = specular;
			triangle.p[0] = v1;
			triangle.p[1] = v2;
			triangle.p[2] = v3;
			triangle.color = this->color;
			triangle.normal = normals.at(face.index[0].norm - 1);
			triangles.push_back(triangle);
		}
		for (const Vector& vertex : vertices) {
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

		boundingBox.lowest = lowest;
		boundingBox.highest = highest;
	}
};
