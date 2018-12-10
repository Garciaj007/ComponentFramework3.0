#ifndef COLLIDER_H
#define COLLIDER_H

#include "Primitive.h"
#include "Matrix2.h"
#include <vector>
#include <set>
#include <map>
using namespace GAME;
using namespace MATH;
class Collider
{
private:
	static Vec3 AClosest, BClosest, contactNormal;
	static std::map<float, std::pair<Vec3, Vec3>> originalpoints;

	static void CalculateContactNormals(Polygon* p1, Polygon* p2);
	static void CalculateImpulse(Primitive* p1, Primitive* p2);
	Mat3 static TST(Polygon* p, float scaleFactor);
public:
	static float Epsilon;

	static void Print();
	static bool Collided(Primitive* p1, Primitive* p2);
	static void HandleCollision(Primitive* p1, Primitive* p2);
	Vec3 static Support(Polygon* p1, Polygon* p2, const Vec3& direction);
};

//GJK Simplex
struct Simplex {
	//Members
	Vec3 A, B, C;
	//Methods
	bool ContainsOrigin(Polygon* p1, Polygon* p2);
};



#endif // !COLLIDER_H
