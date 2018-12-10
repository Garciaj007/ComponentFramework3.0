#ifndef BODY_H
#define BODY_H

#include "Vector.h"
#include "VMath.h"
using namespace MATH;
class Body {
	//Members
private:
	float radius = 0, angularVel = 0, angularAccel = 0;
	Vec3 accel, worldForce, forceApplied;
public: 
	mutable float mass = 0, rotationalInertia = 0, angle = 0;
	mutable Vec3 pos, vel; ///Can only be retrived but not changed unless const funtion is used
	bool gravity;

	//Constructors
	Body(float mass_, float rotationalInertia_);
	Body(float mass_, Vec3 pos_);
	Body(float mass_, Vec3 pos_, Vec3 vel_, Vec3 accel_);
	Body(float mass_, float rotationalInertia_, Vec3 pos_, Vec3 vel_, Vec3 accel_);

	//Member Methods
	void Update(const float deltaTime);
	void ApplyForce(Vec3 force);
	void ApplyTorque(float torque);

	void ApplyImpulse(float impulse, Vec3 contactNormal, Vec3 _radius);

	void SetPosition(const Vec3& pos) const;

	void Print();
};

#endif

