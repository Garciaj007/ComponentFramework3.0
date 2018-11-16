#include "Collider.h"
#include "VMath.h"
#include "MMath.h"
#include <iostream>

//Check for Collisions
bool Collider::Collided(Polygon* p1, Polygon* p2) {
	//Get initial direction from shapes centers
	Vec3 direction = p2->GetCenter() - p1->GetCenter();
	//Simplex object
	Simplex simplex;

	//Get Point A
	simplex.A = Support(p1, p2, direction);

	//Get Point B using the opposite direction
	simplex.B = Support(p1, p2, -direction);

	//Form a line
	Vec3 line = simplex.B - simplex.A;

	//Normal of line AB
	Vec3 n1(-line.y, line.x, line.z);
	Vec3 n2(line.y, -line.x, line.z);

	//Find the mid point of Line AB 
	Vec3 lerp = (-0.5f * simplex.A) + (0.5f * simplex.B);

	//check if the n1 . -lerp > n2 . -lerp & set the direction to the normal
	if (VMath::dot(n1, -lerp) > VMath::dot(n2, -lerp))
		direction = n1;
	else
		direction = n2;

	//Get Point C
	simplex.C = Support(p1, p2, direction);

	//Check if the simplex contains the origin
	if (simplex.ContainsOrigin(p1, p2)) {
		return true;
	}
	else {
		return false;
	}
}

//Handles Collision
void Collider::HandleCollision(Polygon* p1, Polygon* p2) {
	float scale = 1.0f; //The scaling factor

	Matrix4 scaled1, scaled2;//TST outcomes

	//Temporary Polygons, which are the scaled outcomes scaled and translated by the TST
	Polygon* shape1 = new Polygon();
	Polygon* shape2 = new Polygon();

	//A loop to determine if there is still a collision and scale acordingly 
	while (Collided(shape1, shape2)) {
		scale -= 0.5f; //Reduce scale by 5%

		//Clearing all vertices if there are any
		shape1->verticies.clear();
		shape2->verticies.clear();

		//Performing TST returns a Mat4
		scaled1 = TST(p1, scale);
		scaled2 = TST(p2, scale);

		//Pushing Matricies into polygons
		for (auto v : p1->verticies) {
			Vec4 v4(v);
			shape1->verticies.push_back(Vec3(scaled1 * v4));
		}
		for (auto v : p2->verticies) {
			Vec4 v4(v);
			shape2->verticies.push_back(Vec3(scaled2 * v4));
		}
	}

	//Clearing all vertices from support functions before
	originalpoints.clear(); 

	//Retrieving initial direction 
	Vec3 direction = shape2->GetCenter() - shape1->GetCenter();

	//Finding 1st point via support
	Vec3 A = Support(shape1, shape2, direction);

	//Finding 2nd point via support where direction is negated
	Vec3 B = Support(shape1, shape2, -direction);

	//Finding line AB
	Vec3 AB = B - A;

	//Finding t closest to the collision using lerp
	float t = VMath::dot(-A, AB) / VMath::dot(AB, AB);

	//Finding the point on AB closest to origin
	Vec3 Q1 = A + (B * t);

	//Finding 3rd Point using support function
	Vec3 C = Support(shape1, shape2, -Q1);

	//Finding line BC
	Vec3 BC = C - B;

	//Finding line AC
	Vec3 AC = C - A;

	//Finding the point on AC closest to the origin
	Vec3 Q2 = A + ((VMath::dot(-A, AC) / VMath::dot(AC, AC)) * AC);

	//Finding the point on BC closest to the origin
	Vec3 Q3 = B + ((VMath::dot(-B, BC) / VMath::dot(BC, BC)) * BC);

	//Magnitude of closest points 
	float Q1Mag = VMath::mag(Q1);
	float Q2Mag = VMath::mag(Q2);
	float Q3Mag = VMath::mag(Q3);

	//Finding if there is another point closer to the origin
	if (Q1Mag < Q2Mag && Q1Mag < Q3Mag) {
		A = Support(shape1, shape2, -Q1);
	}

	//Finding if there is another point closer to the origin
	if (Q2Mag < Q1Mag && Q2Mag < Q3Mag) {
		B = Support(shape1, shape2, -Q2);
	}

	//Finding if there is another point closer to the origin
	if (Q3Mag < Q1Mag && Q3Mag < Q2Mag) {
		C = Support(shape1, shape2, -Q3);
	}

	//Finding L 
	Vec3 L = B - A;

	//Finding Lambda 1 using M2 . L / L . L
	float lambda1 = VMath::dot(B, L) / VMath::dot(L, L);

	//Finding Lambda 2 using 1 - lambda1
	float lambda2 = 1 - lambda1;

	//Finding A Closest using Lambda1 * A1 + Lambda2 * A2
	Vec3 AClosest = lambda1 * originalpoints[A].first + lambda2 * originalpoints[B].first;

	//Finding B Closest using Lambda1 * B1 + Lambda2 * B2
	Vec3 BClosest = lambda1 * originalpoints[A].second + lambda2 * originalpoints[B].second;

	//Finding Contact Normal
	Vec3 contactNormal = (BClosest - AClosest) / (VMath::mag(BClosest - AClosest));

	std::cout << "Contact Normal : " << contactNormal << std::endl;

	//Converting to vector4 for matrix multiplication
	Vec4 Ac4(AClosest);
	Vec4 Bc4(BClosest);

	//Finding the Contact Points on Original Shape using inverse of TST * Closest Point
	Vec3 AclosestOriginal = MMath::inverse(scaled1) * Ac4;
	Vec3 BclosestOriginal = MMath::inverse(scaled2) * Bc4;

	std::cout << "Closest point on A : " << AclosestOriginal << std::endl;
	std::cout << "Closest point on B : " << BclosestOriginal << std::endl;
}

//Get a point on the simplex
Vec3 Collider::Support(Polygon* p1, Polygon* p2,const Vec3& direction) {
	//Get the farthest point in the direction
	Vec3 point1 = p1->GetBiggestPoint(direction);
	Vec3 point2 = p2->GetSmallestPoint(direction);

	//Minkowski Sum
	Vec3 sum = point1 - point2;

	//emplacing into map for future use
	originalpoints.emplace(sum, std::make_pair(point1, point2));

	//returning sum
	return sum;
}

//Performs the translation to centre, scaling, and translation to origin
Matrix4 Collider::TST(Polygon* p, const float scaleFactor) {
	Matrix4 translate(MMath::translate(p->GetCenter().x, p->GetCenter().y, 1));
	Matrix4 scale(MMath::scale(scaleFactor, scaleFactor, 1));
	Matrix4 origin(MMath::translate(-p->GetCenter().x, -p->GetCenter().y, 1));

	return Matrix4(translate * scale * origin);
}

//------------------------------------------------------------- Simplex ---------------------------------------------------------------//

//return true if collision has occured, or false
bool Simplex::ContainsOrigin(Polygon* p1, Polygon* p2) {
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
	if (VMath::dot(ACPerp, -B) < 0)
		return false;

	//find a vector perpendicular to BC
	Vec3 BCPerp = VMath::cross(VMath::cross(BC, BA), BC);

	//if BCPerp . BO < 0 collision has not occurred
	if (VMath::dot(BCPerp, -A) < 0) {
		//Searching along BCPerp to find another point
		Vec3 Atemp = Collider::Support(p1, p2, -BCPerp);
		
		//if A != A, Check again if positive, else collision has not occured
		if (Atemp != A && VMath::dot(BCPerp, -A) >= 0)
			A = Atemp;
		else 
			return false;
	}

	//return true if the above does not apply
	return true;
}



