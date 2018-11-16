#include "Polygon.h"
#include "VMath.h"

Polygon::Polygon(std::vector<Vec3> verticies_) {
	verticies = verticies_;
}

Vec3 Polygon::GetCenter() {
	Vec3 center;
	for (auto v : verticies) {
		center += v;
	}
	return center / verticies.size();
}

Vec3 Polygon::GetBiggestPoint(const Vec3& direction) {
	Vec3 point;
	for (auto vertex : verticies) {
		if (VMath::dot(vertex, direction) > VMath::dot(point, direction))
			point = vertex;
	}
	return point;
}

Vec3 Polygon::GetSmallestPoint(const Vec3& direction) {
	Vec3 point;
	for (auto vertex : verticies) {
		if (VMath::dot(vertex, direction) < VMath::dot(point, direction))
			point = vertex;
	}
	return point;
}
