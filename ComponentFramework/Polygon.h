#ifndef POLYGON_H
#define POLYGON_H

#include "Vector.h"
#include <vector>

using namespace MATH;
struct Polygon {
	Polygon() { verticies.clear(); }
	Polygon(std::vector<Vec3> verticies_);
	std::vector<Vec3> verticies;
	virtual Vec3 GetBiggestPoint(const Vec3& direction);
	virtual Vec3 GetSmallestPoint(const Vec3& direction);
	virtual Vec3 GetCenter();

	inline void AddPoint(Vec3& point) {
		verticies.push_back(point);
	}
};

#endif // !POLYGON_H

