#ifndef COLLIDER_H
#define COLLIDER_H

#include "Primitive.h"
using namespace GAME;
class Collider
{
public:
	static bool Collided(Primitive* p1, Primitive* p2);
	static void HandleCollision(Primitive* p1, Primitive* p2);
private:
	Vec3 static Support(Primitive* p1, Primitive* p2, const Vec3& direction);
};

//GJK Simplex
struct Simplex {
	//Members
	Vec3 A, B, C;
	//Methods
	bool ContainsOrigin();
};

#endif // !COLLIDER_H
