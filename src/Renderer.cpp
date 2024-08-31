#ifndef RENDERER_CPP
#define RENDERER_CPP
#include "Window.cpp"
#define WHITE {255,255,255}
internal void clearScreen(u32 color) {
	u32* pixel = (u32*)renderState.memory;
	for (int y = 0; y < renderState.height; y++) {
		for (int x = 0; x < renderState.width; x++) {
 			*pixel++ = color;
		}
	}
}

internal void putPixel(int x, int y,Colour color) {
	u32 hexColor = rgbtoHex(color);
	x += renderState.width / 2; 
	y = (renderState.height / 2) - y;
	u32* pixel = (u32*)renderState.memory + x + (y*renderState.width);
	*pixel = hexColor;
}
internal Colour getPixel(int x, int y){
	u32* pixel = (u32*)renderState.memory + x + (y * renderState.width);
	Colour result = hexToRGB(*pixel);
	return result;
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
internal void exportToPPM(std::string filename) {
	std::ofstream ofs;
	ofs.open(filename);
	if (!ofs.is_open()) {
		std::cout << "Failed to open file : " << filename << "\n";
		return;
	}
	ofs << "P3\n"<< renderState.width << ' ' << renderState.height << "\n255\n";
	for (int y = 0; y < renderState.height; y++) {
		for (int x = 0; x < renderState.width; x++) {
			Colour color = getPixel(x, y);
			ofs << (u32)color.R << ' ' << (u32)color.G << ' ' << (u32)color.B << '\n';
		}
	}
	std::cout << "Exported to PPM successfully : " << filename << "\n";
	ofs.close();
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
				//continue;
			}
			else {
				putPixel(x, y, color);
				y += aspectRatio;
			}
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
			}
			else {
				putPixel(x, y, color);
				x += aspectRatio;
			}
		}
	}
}
Mesh loadOBJ(std::string filename, Colour color = { 0,0,0 }, double reflectiveness = 0,double specular = -1)
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
		std::cout << "Cannot open file " << filename << "\n";
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
	mesh.color = color;
	mesh.specular = specular;
	mesh.reflectiveness = reflectiveness;
	mesh.initTriangles();
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
internal void drawTriangle(Triangle t,bool wireframe = false) {
	//for (int i = 0; i < 3; i++) {
	//	if (t.p[i].x <= -(canvas.x / 2.f) || t.p[i].x >= (canvas.x / 2.f) || t.p[i].y <= -(canvas.y / 2.f) || t.p[i].y >= (canvas.x / 2.f)) {
	//		return;
	//	}
	//}
	Vector p1 = t.p[0];// *Vector{ (canvas.x / 2.f),(canvas.y / 2.f),0 };
	Vector p2 = t.p[1];// *Vector{ (canvas.x / 2.f),(canvas.y / 2.f),0 };
	Vector p3 = t.p[2];// *Vector{ (canvas.x / 2.f),(canvas.y / 2.f),0 };
	Colour color = t.color;
	
	if (p1.y > p2.y)swap(p1,p2);
	if (p1.y > p3.y)swap(p1, p3);
	if (p2.y > p3.y)swap(p2, p3);

	if (wireframe) {
		//Skipping pixels outside of screen
		/*if (p1.x > (canvas.x / 2.f) || p1.y >= (canvas.y / 2.f) || p1.x <= -(canvas.x / 2.f) || p1.y <= -(canvas.y / 2.f)) {
			return;
		}
		else if (p2.x > (canvas.x / 2.f) || p2.y >= (canvas.y / 2.f) || p2.x <= -(canvas.x / 2.f) || p2.y <= -(canvas.y / 2.f)) {
			return;
		}
		else if (p3.x > (canvas.x / 2.f) || p3.y >= (canvas.y / 2.f) || p3.x <= -(canvas.x / 2.f) || p3.y <= -(canvas.y / 2.f)) {
			return;
		}*/
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
	std::vector<double> xLeft = {};
	std::vector<double> xRight = {};
	std::vector<double> hLeft = {};
	std::vector<double> hRight = {};
	if (x02.size()) {
		if (x02[m] < x01[m]) {
			xLeft = x02;
			xRight = x01;

			hLeft = h02;
			hRight = h01;
		}
		else {
			xLeft = x01;
			xRight = x02;

			hLeft = h01;
			hRight = h02;
		}
	}
	for (int y = (p1.y); y < p3.y; y++) {
		double xL = xLeft[y - int(p1.y)];
		double xR = xRight[y - int(p1.y)];

		std::vector<double> hSegment = {};
		interpolate(hLeft[y - int(p1.y)], xL, hLeft[y - int(p1.y)], xR, hSegment);
		for (int x = int(xL); x < xR; x++) {
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
		p[i] = projectVertex(p[i]);
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
	Vector N = triangle.getNormal();
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
	Vector pLine = P - triangle.p[0];
	C = cross(edge, pLine);

	if (dot(N, C) < 0)
		return infinity; //Point is outside/Rightside edge 0;
	
	//edge 1
	edge = triangle.p[2] - triangle.p[1];
	pLine = P - triangle.p[1];
	C = cross(edge, pLine);

	if (dot(N, C) < 0)
		return infinity; //Point is outside/Rightside edge 1;

	//edge 2
	edge = triangle.p[0] - triangle.p[2];
	pLine = P - triangle.p[2];
	C = cross(edge, pLine);

	if (dot(N, C) < 0)
		return infinity; //Point is outside/Rightside edge 2;

	return t;
}
bool RayIntersectsBox(Vector& O, Vector& D, Box& box)
{
	Vector invDir = 1.0f / D;
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
		return false;
	}
	if (tzmin > tmin)tmin = tzmin;
	if (tzmax < tmax)tmax = tzmax;
	if (tmin < 0) {
		return false;
	}
	return true;
}
internal std::pair<Object*, double> closestIntersection(Vector O, Vector D, double tMin, double tMax) {
	double closestT = infinity;
	Object* closestObject = nullptr;
	//Sphere intersection
	for (Sphere& sphere : scene.spheres) {
		double sphereInt = intersectRaySphere(O, D, sphere);
		double T = sphereInt;
		if (isIn(T, tMin, tMax) && T < closestT) {
			closestT = T;
			closestObject = &sphere;
		}
	}
	//Triangle
	for (Triangle& triangle : scene.triangles)
	{
		double triangleInt = intersectRayTriangle(O, D, triangle);
		if (isIn(triangleInt, tMin, tMax) && triangleInt < closestT) {
			closestT = triangleInt;
			closestObject = &triangle;
		}
	}
	//Mesh
	for (Instance& instance : scene.instances)
	{
		std::vector<Triangle>& triangles = instance.mesh->triangles;
		Box mbb = instance.getBoundingBox();
		if (debugState == DebugState::DS_BOUNDING_BOX)
		{
			if (!RayIntersectsBox(O, D, mbb))
			{
				continue;
			}
			else {
				closestObject = &tempTri;
				closestT = 0;
				return { closestObject,closestT };
			}
		}
		else {
			if (!RayIntersectsBox(O, D, mbb))
			{
				continue;
			}
		}
		for (Triangle& triangle : triangles)
		{
			Triangle tri;
			tri.p[0] = transformVertex(triangle.p[0], instance.transform);
			tri.p[1] = transformVertex(triangle.p[1], instance.transform);
			tri.p[2] = transformVertex(triangle.p[2], instance.transform);
			Vector normal = tri.getNormal();
			if (dot(normal, D) > 0) {
				continue;
			}
			double triangleInt = intersectRayTriangle(O, D, tri);
			if (isIn(triangleInt, tMin, tMax) && triangleInt < closestT) {
				closestT = triangleInt;
				closestObject = &triangle;
			}
		}
	}
	return { closestObject,closestT };
}
internal Vector reflectRay(Vector R, Vector N) {
	return (2 * (N * dot(R, N)) - R);
}
internal double computeLight(Vector P,Vector N,Vector V,double s) {
	double i = 0.f;
	for (const Light& light : scene.lights) {
		//L = direction of the light
		Vector L = {};
		double tMax=0;
		if (light.type == LT_AMBIENT) {
			i += light.intensity;
		}
		else {
			if (light.type == LT_DIRECTIONAL) {
				L = light.direction;
				tMax = infinity;
			}
			else if (light.type == LT_POINT) {
				L = light.pos - P;
				tMax = 2;
			}
			double shadowT = closestIntersection(P, L, 0.0001, tMax).second;
			if (shadowT != infinity) {
				continue;
			}
			//Diffuse reflection
			double nDotL = dot(N, L);
			if (nDotL > 0) { 
				i += (light.intensity * nDotL / (length(N) * length(L)));
			}
			//specular reflection
			if (s != -1) {
				Vector R = reflectRay(L,N);
				double vDotR = dot(V, R);
				if (vDotR > 0) {
					i += light.intensity * pow((vDotR / (length(R) * length(V))), s);
				}
			}
		}
	}
	return i;
}
Vector rotate(Vector& vec,const Vector& rotation) {
	
	//rotation aruond x axis
	Vector xrotated;

	xrotated.x = vec.x;
	xrotated.y = (vec.y * cos(rotation.x)) + (vec.z * -sin(rotation.x));
	xrotated.z = (vec.y * (sin(rotation.x))) + (vec.z * cos(rotation.x));

	Vector yrotated;

	yrotated.y = xrotated.y;
	
	yrotated.x = xrotated.x * cos(rotation.y) + xrotated.z * (-sin(rotation.y));
	yrotated.z = xrotated.x * sin(rotation.y) + xrotated.z * cos(rotation.y);

	Vector zrotated;
	zrotated.z = yrotated.z;

	zrotated.x = yrotated.x * cos(rotation.z) + yrotated.y * (-sin(rotation.z));
	zrotated.y = yrotated.x * sin(rotation.z) + yrotated.y * cos(rotation.z);

	return zrotated;
}
Vector transformVertex(Vector vec,const Transform& tf) {
	Vector rotated = rotate(vec, tf.rotation);
	Vector translated = (rotated * tf.scale) + tf.position;
	return translated;
}
void renderObject(Instance& instance,bool bfc = true) {
	std::vector<Triangle> triangles = instance.mesh->triangles;
	for (Triangle& triangle : triangles) {
		Vector transformed[3];
		Vector moved[3];
		transformed[0] = transformVertex(triangle.p[0],instance.transform);
		transformed[1] = transformVertex(triangle.p[1],instance.transform);
		transformed[2] = transformVertex(triangle.p[2],instance.transform);
		moved[0] = transformed[0] - camera.position;
		moved[1] = transformed[1] - camera.position;
		moved[2] = transformed[2] - camera.position;
		moved[0] = rotate(moved[0], -camera.rotation);
		moved[1] = rotate(moved[1], -camera.rotation);
		moved[2] = rotate(moved[2], -camera.rotation);
		
		if (moved[0].z <= 0 || moved[1].z <= 0 || moved[2].z <= 0) {
			//Triangle is behind camera
			continue;
		}
		
		Vector projected[3];
		projected[0] = projectVertex(moved[0]);
		projected[1] = projectVertex(moved[1]);
		projected[2] = projectVertex(moved[2]);
		Triangle newTri;
		newTri.p[0] = projected[0];
		newTri.p[1] = projected[1];
		newTri.p[2] = projected[2];
		//Backface culling
		const bool backFaceCulling = bfc;
		Triangle transformedTri;
		transformedTri.p[0] = transformed[0];
		transformedTri.p[1] = transformed[1];
		transformedTri.p[2] = transformed[2];
		Vector normal = transformedTri.getNormal();
		normal = normal / length(normal);
		
		//Normal Colouring
		Colour normalCol = { u8(abs(normal.x * 255.f)), u8(abs(normal.y * 255.f)), u8(abs(normal.z * 255.f)) };
		newTri.color = normalCol;// *computeLight(moved[0], normal, -D, instance.mesh->specular);
		Vector PO = -moved[0];
		PO = rotate(PO, camera.rotation);
		if ((dot(normal, PO) > 0) || !backFaceCulling) {
			bool drawWireframe = false;
			if (debugState == DebugState::DS_BOUNDING_BOX) {
				Box box = instance.getBoundingBox();
				box.highest = box.highest - camera.position;
				box.lowest = box.lowest - camera.position;
				box.highest = rotate(box.highest, -camera.rotation);
				box.lowest = rotate(box.lowest, -camera.rotation);
				drawBox(box);
				return;
			}
			if (debugState == DebugState::DS_TRIANGLE)drawWireframe = true;
			drawTriangle(newTri, drawWireframe);
		}
	}
}
internal Colour traceRay(Vector O,Vector D,double tMin,double tMax,int recursionLimit){
	std::pair<Object*, double> intersection = closestIntersection(O , D, tMin, tMax);
	Object* closestObject = intersection.first;
	double closestT = intersection.second;
	Colour bgColor = { 100,100,100 };
	if (closestT == infinity) {
		return bgColor;
	}
	//P = point of the intersection
	Vector P = O + (D * closestT);
	//N = normal at the point
	Vector N = {};
	if (closestObject->getType() == Type::ST_SPHERE)
	{
		N = P - ((Sphere*)(closestObject))->center;
	}
	else if (closestObject->getType() == Type::ST_TRIANGLE)
	{
		N = ((Triangle*)(closestObject))->getNormal();
	}
	//Normalizing the normal
	N = N / length(N);
	double light = computeLight(P, N, -D, closestObject->specular);
	Colour localColor = (closestObject->color * light);
	double r = closestObject->reflectiveness;
	if (recursionLimit <= 0 || r <= 0.f) {
		return localColor;
	}

	Vector R = reflectRay(-D, N);
	Colour reflectedColor = traceRay(P, R, 0.001, infinity, recursionLimit - 1);

	return (localColor * (1.f - r)) + (reflectedColor * r);
}
void rayTraceThr(int threadNum,int threadCount)
{
	float ycount = (canvas.y / threadCount);
	float ymin = ycount * threadNum;
	float ymax = ymin + ycount;
	ymin -= canvas.y / 2.0f;
	ymax -= canvas.y / 2.0f;
	for (float y =ymin; y < ymax; y++) {
		int scanlineDone = ( canvas.y - (y + ((canvas.y) / 2.0f)));
		for (float x = (-canvas.x / 2); x < (canvas.x / 2); x++) {
			D = canvasToViewport(x, y);
			D = D / length(D);
			D = rotate(D, camera.rotation);
			Colour result = traceRay(camera.position, D, 1, infinity, 3);
			putPixel(x, y, result);
		}
	}
}	
void rayTrace() {
	clearScreen(0x000000);
	for (float y = (canvas.y / 2); y > -(canvas.y / 2); y--) {
		int scanlineDone = ( canvas.y - (y + ((canvas.y) / 2.0f)));
		std::cout << "\rScanlines Done:" << scanlineDone << '/' << (canvas.y - 1) << ':' << int((scanlineDone / (canvas.y - 1)) * 100) << "%" << std::flush;
		for (float x = (-canvas.x / 2); x < (canvas.x / 2); x++) {
			D = canvasToViewport(x, y);
			D = D / length(D);
			D = rotate(D, camera.rotation);
			Colour result = traceRay(camera.position, D, 1, infinity, 3);
			putPixel(x, y, result);
		}
	}
}
#endif