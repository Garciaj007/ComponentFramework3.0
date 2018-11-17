#ifndef BODY_H
#define BODY_H
#include "Vector.h"
using namespace MATH;
class Body {
	//Members
private:
	float radius, mass, rotationalInertia, angle, angularVel, angularAccel;
	Vec3 accel, worldForce, forceApplied;
public: 
	mutable Vec3 pos, vel; ///Can only be retrived but not changed unless const funtion is used
	bool gravity;

	//Constructors
	Body(float mass_, float rotationalInertia_);
	Body(float mass_, Vec3 pos_);
	Body(float mass_, Vec3 pos_, Vec3 vel_, Vec3 accel_);

	//Member Methods
	void Update(const float deltaTime);
	void ApplyForce(Vec3 force);
	void ApplyTorque(float torque);

	void SetPosition(const Vec3& pos) const;
};

#endif

