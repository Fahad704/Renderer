#pragma once
#include "Object.h"
#include "Transform.h"
struct Instance {
	Mesh* mesh;
	Transform transform;
	Box boundingBox;
	Instance(Mesh& mesh, const Vector& position = { 0,0,0 }, float scale = 1, const Vector& rotation = { 0,0,0 }) {
		this->mesh = &mesh;
		transform.position = position;
		transform.scale = scale;
		transform.rotation = rotation;
		boundingBox.lowest = INFINITY;
		boundingBox.highest = -INFINITY;
	}
	//Returns Bounding Box in world space
	Box getBoundingBox() {
		if ((boundingBox.lowest == INFINITY) && (boundingBox.highest == -INFINITY)) {
			Vector lowest = { INFINITY,INFINITY,INFINITY };
			Vector highest = { -INFINITY,-INFINITY,-INFINITY };
			for (const Triangle& triangle : mesh->triangles) {
				Vector tv[3];
				tv[0] = transformVertex(triangle.p[0], transform);
				tv[1] = transformVertex(triangle.p[1], transform);
				tv[2] = transformVertex(triangle.p[2], transform);
				for (int i = 0; i < 3; i++) {
					lowest.x = (tv[i].x < lowest.x ? tv[i].x : lowest.x);
					lowest.y = (tv[i].y < lowest.y ? tv[i].y : lowest.y);
					lowest.z = (tv[i].z < lowest.z ? tv[i].z : lowest.z);

					highest.x = (tv[i].x > highest.x ? tv[i].x : highest.x);
					highest.y = (tv[i].y > highest.y ? tv[i].y : highest.y);
					highest.z = (tv[i].z > highest.z ? tv[i].z : highest.z);
				}
			}

			boundingBox = { highest, lowest };
			return { highest,lowest };
		}
		else {
			return boundingBox;
		}
	}
	void applyTransform(const Transform& tf) {
		transform.scale = tf.scale * transform.scale;
		transform.position = tf.position + transform.position;
		transform.rotation = tf.rotation + transform.rotation;
		boundingBox.lowest = INFINITY;
		boundingBox.highest = -INFINITY;
		getBoundingBox();
	}

};
enum LightType {
	LT_POINT,
	LT_DIRECTIONAL,
	LT_AMBIENT
};
struct Light {
	LightType type;
	Vector pos;
	Vector direction;
	float intensity;
};
struct Scene {
	std::vector<Sphere> spheres;
	std::vector<Triangle> triangles;
	std::vector<Instance> instances;
	std::vector<Light> lights;
};