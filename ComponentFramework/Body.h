#ifndef BODY_H
#define BODY_H

#include "Vector.h"
#include "VMath.h"
using namespace MATH;
class Body {
	//Members
private:
	float angle = 0, angularVel = 0, angularAccel = 0, mass = 0, rotationalInertia = 0;
	Vec3 pos, vel, accel, worldForce, forceApplied;
public: 
	bool gravity;

	//Constructors
	Body(float mass_, float rotationalInertia_);
	Body(float mass_, Vec3 pos_);
	Body(float mass_, Vec3 pos_, Vec3 vel_, Vec3 accel_);
	Body(float mass_, float rotationalInertia_, Vec3 pos_, Vec3 vel_, Vec3 accel_);

	//Getters & Setters
	Vec3 GetPos() const;
	Vec3 GetVel() const;
	Vec3 GetAccel() const;
	Vec3 GetWorldForce() const;
	float GetRotationalInertia() const;
	float GetMass() const;
	float GetAngle() const;
	float GetAngularVelocity() const;
	float GetAngularAccel() const;
	void SetPosition(Vec3 pos_);
	void SetVelocity(Vec3 vel_);
	void SetAngualarVelocity(float angularVel_);

	//Member Methods
	void Update(const float deltaTime);
	void ApplyForce(Vec3 force);
	void ApplyTorque(float torque);
	void ApplyImpulse(float impulse, Vec3 contactNormal, Vec3 _radius);
	void Print();
};

#endif

