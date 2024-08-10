#pragma once
#include "Window.cpp"
#define WHITE {255,255,255}
internal void clearScreen(u32 color) {
	u32* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
 			*pixel++ = color;
		}
	}
}
internal void exportToPPM() {
	//TODO : implement ppm image rendering
}
internal void putPixel(int x, int y,Colour color) {
	u32 hex_color = rgbtoHex(color);
	x += render_state.width / 2; 
	y = (render_state.height / 2) - y;
	u32* pixel = (u32*)render_state.memory + x + (y*render_state.width);
	*pixel = hex_color;
}
internal void drawSquare(double x,double y,int size, Colour color) {
	x -= size * 0.5f;
	y -= size * 0.5f;
	for (int i = y; i < y + size; i++) {
		for (int j = x; j < x + size; j++) {
			putPixel(j, i, color);
		}
	}
}
internal void drawLine(Vector a, Vector b, Colour color) {
	double dy = b.y - a.y;
	double dx = b.x - a.x;
	if (abs(dx) > abs(dy)) {
		if (a.x > b.x) {
			swap(a, b);
		}
		double aspectRatio=0;
		if(dx != 0)
			aspectRatio = (dy / dx);
		double y = a.y;
		for (int x = a.x; x <= b.x; x++) {
			if (x >= (canvas.x / 2) || x <= -(canvas.x / 2) || y >= (canvas.y / 2) || y <= -(canvas.y / 2)) {
				y += aspectRatio;
				continue;
			}
			putPixel(x, y, color);
			y += aspectRatio;
		}
	}
	else {
		if (a.y > b.y) {
			swap(a, b);
		}
		double aspectRatio = 0;
		if (dy != 0)
			aspectRatio = (dx / dy);
		double x = a.x;
		for (int y = a.y; y <= b.y; y++) {
			if (x >= (canvas.x / 2) || x <= -(canvas.x / 2) || y >= (canvas.y / 2) || y <= -(canvas.y / 2)) {
				x += aspectRatio;
				continue;
			}
			putPixel(x, y, color);
			x += aspectRatio;
		}
	}
}
Mesh loadOBJ(std::string filename, Vector pos = { 0,0,0 }, Colour color = { 0,0,0 }, double reflectiveness = 0,double specular = -1)
{
	std::vector<Vector> vertexes = {};
	std::vector<Vector> normals = {};
	std::vector<Texture> texture = {};
	std::vector<Face> faces = {};
	enum Modes
	{
		NONE,
		VERTEX_MODE,
		TEXTURE_MODE,
		NORMAL_MODE,
		FACE_MODE
	};
	Modes mode;
	std::ifstream OBJFile;
	std::string line;
	OBJFile.open(filename);
	if (!OBJFile)
	{
		std::cerr << "Cannot open file " << filename << "\n";
		return {};
	}
	std::unordered_map<std::string, Modes> map = {
		{"# ", NONE},
		{"v ", VERTEX_MODE},
		{"vt", TEXTURE_MODE},
		{"vn", NORMAL_MODE},
		{"f ", FACE_MODE} };
	while (std::getline(OBJFile, line))
	{
		std::string m = line.substr(0, 2);
		mode = map[m];
		switch (mode)
		{
		case VERTEX_MODE:
		{
			std::istringstream iss(line.substr(2));
			double x=0, y=0, z=0;
			iss >> x >> y >> z;
			vertexes.push_back({x,y,z});
		}
		break;
		case TEXTURE_MODE:
		{
			std::istringstream iss(line.substr(3));
			double u, v, w;
			iss >> u >> v >> w;
			Texture newtext({ u, v, w });
			texture.push_back(newtext);
		}
		break;
		case NORMAL_MODE:
		{
			std::istringstream iss(line.substr(3));
			double x, y, z;
			iss >> x >> y >> z;
			Vector newnorm(x, y, z);
			normals.push_back(newnorm);
		}
		break;
		case FACE_MODE:
		{
			std::istringstream iss(line.substr(2));
			std::string s[3];
			iss >> s[0] >> s[1] >> s[2];
			Index index[3];

			for (int i = 0; i < 3; i++)
			{
				std::string str = "";
				int ind[3] = {};
				int k = 0;
				for (int j = 0; j < s[i].length(); j++)
				{
					if (s[i][j] == '/')
					{
						if (str.length())
						{
							std::istringstream iss(str);
							iss >> ind[k];
						}
						else {
							ind[k] = -1;//no index
						}
						str = "";
						k++;
						continue;
					}
					str += s[i][j];
				}
				index[i] = { ind[0],ind[1],ind[2] };
			}
			Face newface = { index[0],index[1],index[2] };
			faces.push_back(newface);
		}
		break;
		default:
		{
			continue;
		}
		break;
		}
	}
	OBJFile.close();
	Mesh mesh = { vertexes,normals,texture,faces};
	if (pos == Vector{0, 0, 0}) {
		return mesh;
	}
	mesh.setPos(pos, color);
	mesh.specular = specular;
	mesh.reflectiveness = reflectiveness;
	return mesh;
}
Vector canvasToViewport(double x,double y) {
	return { x * (vpWidth / canvas.x), y * (vpHeight / canvas.y), d };
}
std::pair<double, double> viewportToCanvas(double x, double y) {
	return { x * (canvas.x / vpWidth) ,y * (canvas.y / vpHeight) };
}
Vector projectVertex(Vector v) {
	//Perspective Projection
	std::pair<double, double> result = viewportToCanvas(double(v.x * d / v.z), double(v.y * d / v.z));
	return { result.first,result.second ,d};
}
void interpolate(double x0, int y0, double x1, double y1,std::vector<double>& arr) {
	double dx = x1 - x0;
	double dy = y1 - y0;
	double aspectratio = 0;
	double x = x0;
	if (dy != 0) 
		aspectratio = (dx / dy);
	for (double y = y0; y <= y1; y++) {
		arr.push_back(x);
		x += aspectratio;
	}
}
internal void draw_triangle(Triangle t,bool wireframe = false) {
	for (int i = 0; i < 3; i++) {
		if (t.p[i].x <= -(canvas.x / 2.f) || t.p[i].x >= (canvas.x / 2.f) || t.p[i].y <= -(canvas.y / 2.f) || t.p[i].y >= (canvas.x / 2.f)) {
			return;
		}
	}
	Vector p1 = t.p[0] * Vector{ (canvas.x / 2.f),(canvas.y / 2.f),0 };
	Vector p2 = t.p[1] * Vector{ (canvas.x / 2.f),(canvas.y / 2.f),0 };
	Vector p3 = t.p[2] * Vector{ (canvas.x / 2.f),(canvas.y / 2.f),0 };
	Colour color = t.color;
	
	if (p1.y > p2.y)swap(p1,p2);
	if (p1.y > p3.y)swap(p1, p3);
	if (p2.y > p3.y)swap(p2, p3);

	if (wireframe) {
		//Skipping pixels outside of screen
		if (p1.x > (canvas.x / 2.f) || p1.y >= (canvas.y / 2.f) || p1.x <= -(canvas.x / 2.f) || p1.y <= -(canvas.y / 2.f)) {
			return;
		}
		else if (p2.x > (canvas.x / 2.f) || p2.y >= (canvas.y / 2.f) || p2.x <= -(canvas.x / 2.f) || p2.y <= -(canvas.y / 2.f)) {
			return;
		}
		else if (p3.x > (canvas.x / 2.f) || p3.y >= (canvas.y / 2.f) || p3.x <= -(canvas.x / 2.f) || p3.y <= -(canvas.y / 2.f)) {
			return;
		}
		//Drawing wireframe
		drawLine(p1, p2, color);
		drawLine(p2, p3, color);
		drawLine(p3, p1, color);
		return;
	}
	std::vector<double> x01;
	std::vector<double> x12;
	std::vector<double> x02;

	std::vector<double> h01;
	std::vector<double> h12;
	std::vector<double> h02;

	interpolate(p1.x, p1.y, p2.x, p2.y, x01);
	interpolate(p2.x, p2.y, p3.x, p3.y, x12);
	interpolate(p1.x, p1.y, p3.x, p3.y, x02);

	interpolate(0, p1.y, 0, p2.y, h01);
	interpolate(0, p2.y, 1, p3.y, h12);
	interpolate(0, p1.y, 1, p3.y, h02);

	//concatenate short sides doubleo x01
	for (const double& val : x12) {
		x01.push_back(val);
	}
	for (const double& val : h12) {
		h01.push_back(val);
	}
	double m = floor(x02.size() / 2);
	std::vector<double> x_left = {};
	std::vector<double> x_right = {};
	std::vector<double> h_left = {};
	std::vector<double> h_right = {};
	if (x02.size()) {
		if (x02[m] < x01[m]) {
			x_left = x02;
			x_right = x01;

			h_left = h02;
			h_right = h01;
		}
		else {
			x_left = x01;
			x_right = x02;

			h_left = h01;
			h_right = h02;
		}
	}
	for (int y = (p1.y); y < p3.y; y++) {
		double x_l = x_left[y - int(p1.y)];
		double x_r = x_right[y - int(p1.y)];

		std::vector<double> h_segment = {};
		interpolate(h_left[y - int(p1.y)], x_l, h_left[y - int(p1.y)], x_r, h_segment);
		for (int x = int(x_l); x < x_r; x++) {
			if (isIn(double(x), double(-canvas.x / 2.f), double(canvas.x / 2.f)) && isIn(double(y), double(-canvas.y / 2.f), double(canvas.y / 2.f))) {
				putPixel(x, y, color);
			}
		}
	}
}
void drawBox(Box box) {
	Colour red = { 255,0,0 };
	//Front faces
	Vector p[] = {
		//Front Points
		{ box.lowest.x, box.lowest.y, box.lowest.z },
		{ box.lowest.x, box.highest.y, box.lowest.z },
		{ box.highest.x, box.highest.y, box.lowest.z },
		{ box.highest.x, box.lowest.y, box.lowest.z },
		//Back Points
		{ box.lowest.x, box.lowest.y, box.highest.z },
		{ box.lowest.x, box.highest.y, box.highest.z },
		{ box.highest.x, box.highest.y, box.highest.z },
		{ box.highest.x, box.lowest.y, box.highest.z }
	};
	int size = sizeof(p) / sizeof(p[0]);
	for (int i = 0; i < size; i++) {
		p[i] = projectVertex(p[i]-O);
	}
	//Front lines
	drawLine(p[0], p[1], red);
	drawLine(p[1], p[2], red);
	drawLine(p[2], p[3], red);
	drawLine(p[3], p[0], red);
	//Back lines
	drawLine(p[4], p[5], red);
	drawLine(p[5], p[6], red);
	drawLine(p[6], p[7], red);
	drawLine(p[7], p[4], red);
	//Side lines
	drawLine(p[0], p[4], red);
	drawLine(p[1], p[5], red);
	drawLine(p[2], p[6], red);
	drawLine(p[3], p[7], red);
}

double intersectRaySphere(Vector& O,Vector& D,Sphere& sphere) {
	double r = sphere.radius;
	Vector CO = O - sphere.center;

	double a = dot(D, D);
	double b = 2 * dot(CO, D);
	double c = dot(CO, CO) - (r * r);

	double discriminant = (b * b) - (4 * a * c);

	if (discriminant < 0) {
		return infinity;
	}
	double t = (-b - sqrt(discriminant)) / (2 * a);
	return t;
}
internal double intersectRayTriangle(Vector O, Vector D, Triangle triangle)
{
	double t = 0;
	Vector N = triangle.get_normal();
	double NdotRay = dot(N, D);

	if (NdotRay > 0)
		return infinity;
	double d = -dot(N, triangle.p[0]);
	t = -(dot(N, O)+d) / NdotRay;
	if (t < 0)
		return infinity;

	Vector P = O + (t * D);

	Vector C;

	//edge 0
	Vector edge = triangle.p[1] - triangle.p[0];
	Vector Pline = P - triangle.p[0];
	C = cross(edge, Pline);

	if (dot(N, C) < 0)
		return infinity; //Point is outside/Rightside edge 0;
	
	//edge 1
	edge = triangle.p[2] - triangle.p[1];
	Pline = P - triangle.p[1];
	C = cross(edge, Pline);

	if (dot(N, C) < 0)
		return infinity; //Point is outside/Rightside edge 1;

	//edge 2
	edge = triangle.p[0] - triangle.p[2];
	Pline = P - triangle.p[2];
	C = cross(edge, Pline);

	if (dot(N, C) < 0)
		return infinity; //Point is outside/Rightside edge 2;

	return t;
}
bool RayIntersectsBox(Vector& O, Vector& D, Box& box)
{
	//static bool result = false;
	/*static Vector param_O = {0,0,0};
	static Vector param_D = {0,0,0};
	static Box param_box = {0,0};
	if ((O == param_O) && (D == param_D) && (box == param_box)) {
		return result;
	}*/
	Vector invDir = 1.0f / D;
	/*if (!D.x)invDir.x = INFINITY;
	if (!D.y)invDir.y = INFINITY;
	if (!D.z)invDir.z = INFINITY;*/
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (invDir.x >= 0) {
		tmin = (box.lowest.x - O.x) * invDir.x;
		tmax = (box.highest.x - O.x) * invDir.x;
	}
	else {
		tmin = (box.highest.x - O.x) * invDir.x;
		tmax = (box.lowest.x - O.x) * invDir.x;
	}

	if (invDir.y >= 0) {
		tymin = (box.lowest.y - O.y) * invDir.y;
		tymax = (box.highest.y - O.y) * invDir.y;
	}
	else {
		tymin = (box.highest.y - O.y) * invDir.y;
		tymax = (box.lowest.y - O.y) * invDir.y;
	}

	if ((tmin > tymax) || (tymin > tmax)) {
		//result = false;
		return false;
	}

	if (tymin > tmin)tmin = tymin;
	if (tymax < tmax)tmax = tymax;

	if (invDir.z >= 0) {
		tzmin = (box.lowest.z - O.z) * invDir.z;
		tzmax = (box.highest.z - O.z) * invDir.z;
	}
	else {
		tzmin = (box.highest.z - O.z) * invDir.z;
		tzmax = (box.lowest.z - O.z) * invDir.z;
	}
	if ((tmin > tzmax) || (tmax < tzmin)) {
		//result = false;
		return false;
	}
	if (tzmin > tmin)tmin = tzmin;
	if (tzmax < tmax)tmax = tzmax;
	if (tmin < 0) {
		return false;
	}
	//result = true;
	return true;
}
internal std::pair<Object*, double> ClosestIntersection(Vector O, Vector D, double t_min, double t_max) {
	double closest_t = infinity;
	Object* closest_object = nullptr;
	//Sphere intersection
	for (Sphere& sphere : scene.spheres) {
		double sphere_int = intersectRaySphere(O, D, sphere);
		double T = sphere_int;
		if (isIn(T, t_min, t_max) && T < closest_t) {
			closest_t = T;
			closest_object = &sphere;
		}
	}
	//Triangle
	for (Triangle& triangle : scene.triangles)
	{
		double triangle_int = intersectRayTriangle(O, D, triangle);
		if (isIn(triangle_int, t_min, t_max) && triangle_int < closest_t) {
			closest_t = triangle_int;
			closest_object = &triangle;
		}
	}
	//Mesh
	for (Mesh& mesh : scene.meshes)
	{
		std::vector<Triangle>& triangles = mesh.triangles;
		if (debugState == DebugState::DS_BOUNDING_BOX)
		{
			if (!RayIntersectsBox(O, D, mesh.boundingBox))
			{
				continue;
			}
			else {
				closest_object = &tempTri;
				closest_t = 0;
				return { closest_object,closest_t };
			}
		}
		else {
			if (!RayIntersectsBox(O, D, mesh.boundingBox))
			{
				continue;
			}
		}
		for (Triangle& triangle : triangles)
		{
			if (dot(triangle.normal, D) > 0) {
				continue;
			}
			double triangle_int = intersectRayTriangle(O, D, triangle);
			if (isIn(triangle_int, t_min, t_max) && triangle_int < closest_t) {
				closest_t = triangle_int;
				closest_object = &triangle;
			}
		}
	}
	return { closest_object,closest_t };
}
internal Vector reflectRay(Vector R, Vector N) {
	return (2 * (N * dot(R, N)) - R);
}
internal double computeLight(Vector P,Vector N,Vector V,double s) {
	double i = 0.f;
	for (Light light : scene.lights) {
		//L = direction of the light
		Vector L = {};
		double t_max=0;
		if (light.type == LT_AMBIENT) {
			i += light.intensity;
		}
		else {
			if (light.type == LT_DIRECTIONAL) {
				L = light.direction;
				t_max = infinity;
			}
			else if (light.type == LT_POINT) {
				L = light.pos - P;
				t_max = 1;
			}
			double shadow_t = ClosestIntersection(P, L, 0.0001, t_max).second;
			if (shadow_t != infinity) {
				continue;
			}
			//Diffuse reflection
			double n_dot_l = dot(N, L);
			if (n_dot_l > 0) { 
				i += (light.intensity * n_dot_l / (length(N) * length(L)));
			}
			//specular reflection
			if (s != -1) {
				Vector R = reflectRay(L,N);
				double v_dot_r = dot(V, R);
				if (v_dot_r > 0) {
					i += light.intensity * pow((v_dot_r / (length(R) * length(V))), s);
				}
			}
		}
	}
	return i;
}
void renderObject(Mesh& mesh) {
	std::vector<Triangle> triangles = mesh.triangles;
	for (Triangle& triangle : triangles) {
		Vector projected[3];
		projected[0] = projectVertex(triangle.p[0] - O);
		projected[1] = projectVertex(triangle.p[1] - O);
		projected[2] = projectVertex(triangle.p[2] - O);
		projected[0] = canvasToViewport(projected[0].x, projected[0].y);
		projected[1] = canvasToViewport(projected[1].x, projected[1].y);
		projected[2] = canvasToViewport(projected[2].x, projected[2].y);
		Triangle newTri;
		newTri.p[0] = projected[0];
		newTri.p[1] = projected[1];
		newTri.p[2] = projected[2];
		//Backface culling
		Vector normal = triangle.get_normal();
		normal = normal / length(normal);
		//Normal Colouring
		Colour normalCol = { u8(abs(normal.x * 255.f)), u8(abs(normal.y * 255.f)), u8(abs(normal.z * 255.f)) };
		newTri.color = normalCol * computeLight((triangle.p[0] - O), normal, -D, mesh.specular);
		Vector PO = O - triangle.p[0];
		if (dot(normal, PO) > 0) {
			bool drawWireframe = false;
			if (debugState == DebugState::DS_BOUNDING_BOX) {
				drawBox(mesh.boundingBox);
				return;
			}
			if (debugState == DebugState::DS_TRIANGLE)drawWireframe = true;
			draw_triangle(newTri, drawWireframe);
		}
	}
}
void rasterize() {
	for (Mesh& mesh : scene.meshes) {
		std::vector<Triangle> tris = mesh.triangles;
		for (Triangle& tri : tris) {
			tri.p[0] = (tri.p[0] - O);
			tri.p[1] = (tri.p[1] - O);
			tri.p[2] = (tri.p[2] - O);
			draw_triangle(tri);
		}
	}
}
internal Colour TraceRay(Vector O,Vector D,double t_min,double t_max,int recursion_limit){
	std::pair<Object*, double> intersection = ClosestIntersection(O , D, t_min, t_max);
	Object* closest_object = intersection.first;
	double closest_t = intersection.second;
	Colour bg_colour = { 100,100,100 };
	if (closest_t == infinity) {
		return bg_colour;
	}
	//P = point of the intersection
	Vector P = O + (D * closest_t);
	//N = normal at the point
	Vector N = {};
	if (closest_object->GetType() == Type::ST_SPHERE)
	{
		N = P - ((Sphere*)(closest_object))->center;
	}
	else if (closest_object->GetType() == Type::ST_TRIANGLE)
	{
		N = ((Triangle*)(closest_object))->get_normal();
	}
	//Normalizing the normal
	N = N / length(N);
	double light = computeLight(P, N, -D, closest_object->specular);
	Colour local_color = (closest_object->color * light);
	double r = closest_object->reflectiveness;
	if (recursion_limit <= 0 || r <= 0.f) {
		return local_color;
	}

	Vector R = reflectRay(-D, N);
	Colour reflected_color = TraceRay(P, R, 0.001, infinity, recursion_limit - 1);

	return (local_color * (1.f - r)) + (reflected_color * r);
}
void RayTraceThr(int thread_num,int thread_count)
{
	float ycount = (canvas.y / thread_count);
	float ymin = ycount * thread_num;
	float ymax = ymin + ycount;
	ymin -= canvas.y / 2.0f;
	ymax -= canvas.y / 2.0f;
	for (float y =ymin; y < ymax; y++) {
		int scanline_done = ( canvas.y - (y + ((canvas.y) / 2.0f)));
		for (float x = (-canvas.x / 2); x < (canvas.x / 2); x++) {
			D = canvasToViewport(x, y);
			D = D / length(D);
			Colour result = TraceRay(O, D, 1, infinity, 3);
			putPixel(x, y, result);
		}
	}
}	
void RayTrace() {
	clearScreen(0x000000);
	for (float y = (canvas.y / 2); y > -(canvas.y / 2); y--) {
		int scanline_done = ( canvas.y - (y + ((canvas.y) / 2.0f)));
		std::cout << "\rScanlines Done:" << scanline_done << '/' << (canvas.y - 1) << ':' << int((scanline_done / (canvas.y - 1)) * 100) << "%" << std::flush;
		for (float x = (-canvas.x / 2); x < (canvas.x / 2); x++) {
			D = canvasToViewport(x, y);
			D = D / length(D);
			Colour result = TraceRay(O, D, 1, infinity, 3);
			putPixel(x, y, result);
		}
	}
}