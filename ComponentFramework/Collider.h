#ifndef COLLIDER_H
#define COLLIDER_H

#include "Polygon.h"
#include "Matrix.h"
#include <vector>
#include <set>
#include <map>
using namespace MATH;
class Collider
{
private:
	static std::map<Vec3, std::pair<Vec3, Vec3>> originalpoints;
public:
	static bool Collided(Polygon* p1, Polygon* p2);
	static void HandleCollision(Polygon* p1, Polygon* p2);
	Vec3 static Support(Polygon* p1, Polygon* p2, const Vec3& direction);
	Matrix4 static TST(Polygon* p, float scaleFactor);
};

//GJK Simplex
struct Simplex {
	//Members
	Vec3 A, B, C;
	//Methods
	bool ContainsOrigin(Polygon* p1, Polygon* p2);
};



#endif // !COLLIDER_H
