#include "Collider.h"
#include <iostream>

//Check for Collisions
bool Collider::Collided(Primitive* p1, Primitive* p2) {
	//Get initial direction from shapes centers
	Vec3 direction = p2->GetCenter() - p1->GetCenter();
	//# of iterations
	int iterations = 0;
	//Simplex object
	Simplex simplex;

	//Main iteration loop
	while (true) {
		iterations++;

		//Get Point A
		simplex.A = Support(p1, p2, direction);
		
		//Get Point B using the opposite direction
		simplex.B = Support(p1, p2, -direction);

		//Form a line
		Vec3 line = simplex.B - simplex.A;

		//Normald of line AB
		Vec3 n1(-line.x, line.y, line.z);
		Vec3 n2(line.x, line.y, line.z);

		//Find the mid point of Line AB 
		Vec3 lerp = (-0.5f * simplex.A) + (0.5f * simplex.B);

		//check if the n1 . lerp > n2 . lerp & set the direction to the normal
		if (VMath::dot(n1, -lerp) > VMath::dot(n2, -lerp))
			direction = n1;
		else
			direction = n2;

		//Get Point C
		simplex.C = Support(p1, p2, direction);

		//Check if the simplex contains the origin
		if (simplex.ContainsOrigin()) {
			return true;
		}
		else {
			//Set the direction to the opposite of the initial direction
			direction = p1->GetCenter() - p2->GetCenter();
		}

		//if the # of iterations > 2 collision has not happened, b/c no more simplex shapes can be created
		if (iterations > 2) {
			return false;
		}
	}
}

//Handles Collision
void Collider::HandleCollision(Primitive* p1, Primitive* p2) {
	std::cout << "Collided" << std::endl;
}

//Get a point on the simplex
Vec3 Collider::Support(Primitive* p1, Primitive* p2,const Vec3& direction) {
	//Get the farthest point in the direction
	Vec3 point1 = p1->GetFathestPoint(direction);
	Vec3 point2 = p2->GetFathestPoint(-direction);
	//Minkowski Sum
	return point1 - point2;
}

//return true if collision has occured, or false
bool Simplex::ContainsOrigin() {
	//Get the line vectors for the simplex
	Vec3 AB = B - A;
	Vec3 BA = A - B;
	Vec3 BC = C - A;
	Vec3 AC = C - A;

	//find a vector perpendicular to AB
	Vec3 ABPerp = VMath::cross(VMath::cross(AB, AC),AB);
	
	//if ABPerp . AO < 0 collision has not occurred
	if (VMath::dot(ABPerp, -A) < 0)
		return false;

	//find a vector perpendicular to AC
	Vec3 ACPerp = VMath::cross(VMath::cross(AC, AB), AC);

	//if ACPerp . AO < 0 collision has not occurred
	if (VMath::dot(ACPerp, -A) < 0)
		return false;

	//find a vector perpendicular to BC
	Vec3 BCPerp = VMath::cross(VMath::cross(BC, BA), BC);

	//if BCPerp . BO < 0 collision has not occurred
	if (VMath::dot(BCPerp, -B) < 0) 
		return false;

	//return true if the above does not apply
	return true;
}
