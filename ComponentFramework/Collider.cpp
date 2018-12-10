#include "Collider.h"
#include "VMath.h"
#include <iostream>

std::map<float, std::pair<Vec3, Vec3>> Collider::originalpoints = std::map<float, std::pair<Vec3, Vec3>>();

float Collider::Epsilon;

Vec3 Collider::AClosest;
Vec3 Collider::BClosest;
Vec3 Collider::contactNormal;

//Check for Collisions
bool Collider::Collided(Primitive* p1, Primitive* p2) {
	//Get initial direction from shapes centers
	Vec3 direction = p2->GetCenter() - p1->GetCenter();
	//Simplex object
	Simplex simplex;

	//Get Point A
	simplex.A = Support(p1, p2, direction);

	//Get Point B using the opposite direction
	simplex.B = Support(p1, p2, -direction);

	//Form a line
	Vec3 line = simplex.A - simplex.B;

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
		std::cout << "Polygons have collided..." << std::endl;
		return true;
	}
	else {
		std::cout << "Polygons have NOT collided..." << std::endl;
		return false;
	}
}

//Handles Collision
void Collider::HandleCollision(Primitive* p1, Primitive* p2) {
	
	if (p1->body == nullptr || p2->body == nullptr) {
		return;
	}

	CalculateContactNormals(p1, p2);
	CalculateImpulse(p1, p2);
}

void Collider::CalculateImpulse(Primitive* p1, Primitive* p2) {
	//Finding the Length of collision
	Vec3 r1 = AClosest - p1->GetCenter();
	Vec3 r2 = BClosest - p2->GetCenter();

	//Calculating Impulse
	float impulse = (-(VMath::dot(contactNormal, p1->body->vel - p2->body->vel)) * (Epsilon + 1)) / 
		((1 / p1->body->mass) + (1 / p2->body->mass) + 
			VMath::dot(contactNormal, VMath::cross(VMath::cross(r1, contactNormal) / p1->body->rotationalInertia, r1)) + 
			VMath::dot(contactNormal, VMath::cross(VMath::cross(r2, contactNormal) / p2->body->rotationalInertia, r2)));

	//Applying Impulse to the bodies
	p1->body->ApplyImpulse(impulse, contactNormal, r1);
	p2->body->ApplyImpulse(-impulse, contactNormal, r2);
}

void Collider::CalculateContactNormals(Polygon* p1, Polygon* p2) {
	Mat3 scaled1, scaled2;//TST outcomes

	Vec3 M1, M2;//Points that form the closest line

	//Temporary Polygons, which are the scaled outcomes scaled and translated by the TST
	Polygon* shape1 = new Polygon();
	Polygon* shape2 = new Polygon();

	//Performing TST returns a Mat3
	scaled1 = TST(p1, 0.75f);
	scaled2 = TST(p2, 0.75f);

	//Pushing Matricies into polygons
	for (auto v : p1->verticies) {
		Vec3 result = scaled1 * (Vec3(v.x, v.y, 1));
		shape1->verticies.push_back(Vec3(result.x, result.y, 0));
	}
	for (auto v : p2->verticies) {
		Vec3 result = scaled2 * (Vec3(v.x, v.y, 1));
		shape2->verticies.push_back(Vec3(result.x, result.y, 0));
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
	Vec3 Q1 = A + AB * t;

	//Finding 3rd Point using support function
	Vec3 C = Support(shape1, shape2, -Q1);

	//Finding line AC
	Vec3 AC = C - A;

	t = VMath::dot(-A, AC) / VMath::dot(AC, AC);

	//Finding the point on AC closest to the origin
	Vec3 Q2 = A + AC * t;

	//Finding line BC
	Vec3 BC = C - B;

	t = VMath::dot(-B, BC) / VMath::dot(BC, BC);

	//Finding the point on BC closest to the origin
	Vec3 Q3 = B + BC * t;

	//Magnitude of closest points 
	float Q1Mag = VMath::mag(Q1);
	float Q2Mag = VMath::mag(Q2);
	float Q3Mag = VMath::mag(Q3);

	//Finding if there is another point closer to the origin
	if (Q1Mag < Q2Mag && Q1Mag < Q3Mag) {
		M1 = A;
		M2 = B;
	}

	//Finding if there is another point closer to the origin
	if (Q2Mag < Q1Mag && Q2Mag < Q3Mag) {
		M1 = A;
		M2 = C;
	}

	//Finding if there is another point closer to the origin
	if (Q3Mag < Q1Mag && Q3Mag < Q2Mag) {
		M1 = B;
		M2 = C;
	}

	//Finding L 
	Vec3 L = M2 - M1;

	//Finding Lambda 1 using M2 . L / L . L
	float lambda1 = VMath::dot(M2, L) / VMath::dot(L, L);

	//Finding Lambda 2 using 1 - lambda1
	float lambda2 = 1 - lambda1;

	auto itA = originalpoints.find(VMath::mag(A));
	auto itB = originalpoints.find(VMath::mag(B));

	if (itA != originalpoints.end() && itB != originalpoints.end()) {
		//Finding A Closest using Lambda1 * A1 + Lambda2 * A2
		AClosest = lambda1 * itA->second.first + lambda2 * itB->second.first;

		//Finding B Closest using Lambda1 * B1 + Lambda2 * B2
		BClosest = lambda1 * itA->second.second + lambda2 * itB->second.second;
	}

	//Finding Contact Normal
	contactNormal = (BClosest - AClosest) / VMath::mag(BClosest - AClosest);

	std::cout << "Contact Normal : ";
	contactNormal.print();

	//Finding the Contact Points on Original Shape using inverse of TST * Closest Point
	AClosest = Mat3::Inverse(scaled1) * AClosest;
	BClosest = Mat3::Inverse(scaled2) * BClosest;

	std::cout << "Closest point on A : ";
	AClosest.print();

	std::cout << "Closest point on B : ";
	BClosest.print();

	std::cout << std::endl;

	contactNormal = Vec3(0.707f, 0.707f, 0);
	AClosest = Vec3(1.694f, 2.3f, 1);
	BClosest = Vec3(1.5f, 2, 1);
}

//Get a point on the simplex
Vec3 Collider::Support(Polygon* p1, Polygon* p2, const Vec3& direction) {
	//Get the farthest point in the direction
	Vec3 point1 = p1->GetBiggestPoint(direction);
	Vec3 point2 = p2->GetSmallestPoint(direction);

	//Minkowski Sum
	Vec3 sum = point1 - point2;

	//emplacing into map for future use
	originalpoints.emplace(VMath::mag(sum), std::make_pair(point1, point2));

	//returning sum
	return sum;
}

//Performs the translation to centre, scaling, and translation to origin
Mat3 Collider::TST(Polygon* p, const float scaleFactor) {
	Mat3 translate(Mat3::Translate(p->GetCenter().x, p->GetCenter().y, 1));
	Mat3 scale(Mat3::Scale(scaleFactor, scaleFactor, 1));
	Mat3 origin(Mat3::Translate(-p->GetCenter().x, -p->GetCenter().y, 1));
	return translate * scale * origin;
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
	Vec3 ABPerp = (AC * VMath::dot(AB, AB)) - (AB * VMath::dot(AB, AC));

	//if ABPerp . AO < 0 collision has not occurred
	if (VMath::dot(ABPerp, -A) < 0)
		return false;

	//find a vector perpendicular to AC
	Vec3 ACPerp = (AB * VMath::dot(AC, AC)) - (AC * VMath::dot(AC, AB));

	//if ACPerp . AO < 0 collision has not occurred
	if (VMath::dot(ACPerp, -A) < 0)
		return false;

	//find a vector perpendicular to BC
	Vec3 BCPerp = (BA * VMath::dot(BC, BC)) - (BC * VMath::dot(BC, BA));

	//if BCPerp . BO < 0 collision has not occurred
	if (VMath::dot(BCPerp, -B) < 0) {
		//Searching along BCPerp to find another point
		Vec3 Atemp = Collider::Support(p1, p2, -BCPerp);

		//if A != A, Check again if positive, else collision has not occured
		if (Atemp != A && VMath::dot(BCPerp, -Atemp) >= 0)
			A = Atemp;
		else
			return false;
	}

	//return true if the above does not apply
	return true;
}



