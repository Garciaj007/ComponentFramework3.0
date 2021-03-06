#include "Body.h"
#include <iostream>
#include <iomanip>

/* Constructors */

Body::Body(float mass_, float rotaionalInertia_) {
	mass = mass_;
	rotationalInertia = rotaionalInertia_;
}

Body::Body(float mass_, Vec3 pos_) {
	mass = mass_;
	pos = pos_;
}

Body::Body(float mass_, Vec3 pos_, Vec3 vel_, Vec3 accel_) {
	mass = mass_;
	pos = pos_;
	vel = vel_;
	accel = accel_;
}

Body::Body(float mass_, float rotationalInertia_, Vec3 pos_, Vec3 vel_, Vec3 accel_) {
	mass = mass_;
	rotationalInertia = rotationalInertia_;
	pos = pos_;
	vel = vel_;
	accel = accel_;
}

void Body::SetPosition(const Vec3& pos_) const {
	pos = pos_;
}

/* Member Methods */

//Updates Body
void Body::Update(const float deltaTime) {

	//With Gravity
	if (gravity) {
		//Update Orientation Angle : Angle = initAngle + W(init)*time + 1/2AngularAcel(time^2)
		angle += (angularVel * deltaTime) + (0.5f * angularAccel * (deltaTime * deltaTime));

		//Update Angular Velocity W = W(initial) + AngularAccel*time
		angularVel += angularAccel * deltaTime;

		//Rotate force Vector 
		worldForce.x = (forceApplied.x * cos(angle)) + (forceApplied.y * -sin(angle));
		worldForce.y = (forceApplied.x * sin(angle)) + (forceApplied.y * cos(angle));

		//Apply force 
		accel = worldForce / mass;

		//Update Positon 
		pos.x = (vel.x * deltaTime) + (accel.x * 0.5f * (deltaTime * deltaTime));
		pos.y = (vel.y * deltaTime) + (accel.y * 0.5f * -9.8f * (deltaTime * deltaTime));
		pos.z = (vel.z * deltaTime) + (accel.z * 0.5f * (deltaTime * deltaTime));


		//Velocity
		vel.x += accel.x * deltaTime;
		vel.y += (accel.y * -9.8f) * deltaTime;
		vel.x += accel.z * deltaTime;
	}
	//Without Gravity
	else {

		//Update Orientation Angle : Angle = initAngle + W(init)*time + 1/2AngularAcel(time^2)
		angle += (angularVel * deltaTime) + (0.5f * angularAccel * (deltaTime * deltaTime));

		//Update Angular Velocity W = W(initial) + AngularAccel*time
		angularVel += angularAccel * deltaTime;

		//Rotate force Vector 
		worldForce.x = (forceApplied.x * cos(angle)) + (forceApplied.y * -sin(angle));
		worldForce.y = (forceApplied.x * sin(angle)) + (forceApplied.y * cos(angle));

		//Apply force 
		accel = worldForce / mass;

		//Update Positon 
		pos += (vel * deltaTime) + (accel * 0.5f * (deltaTime * deltaTime));

		//Velocity
		vel += accel * deltaTime;
	}
}

void Body::ApplyImpulse(float impulse, Vec3 contactNormal, Vec3 length) {
	vel += (impulse * contactNormal) / mass;
	angularVel += Vec3(VMath::cross(length, impulse * contactNormal)).z / rotationalInertia;
}

//Sets Force
void Body::ApplyForce(Vec3 force) {
	forceApplied = force;
}

//Applies Torque (Sets Angular Accel)
void Body::ApplyTorque(float torque) {
	angularAccel = torque / rotationalInertia;
}

void Body::Print() {
	pos.print();
	vel.print();
	std::cout << std::fixed << angularVel << std::endl;
}