#include "Matter.h"
#include <vector>

Matter::Matter()
{
	mass = 1.0f;
	radius = 1.0f;

	location = Vector3(0.0f, 0.0f, 0.0f);
	velocity = Vector3(0.0f, 0.0f, 0.0f);
}

float Matter::getDist(Matter body) {
	return Vector3::getDistance(this->location, body.location);
}

Vector3 Matter::getSep(Matter body) {
	return (this->location - body.location);
}

void Matter::updatePos(float dt) {
	float dx = this->velocity.x * dt;
	float dy = this->velocity.y * dt;
	float dz = this->velocity.z * dt;
	this->location = this->location + Vector3(dx, dy, dz);
}

float Matter::getSpeedSquared() {
	return (this->location.x * this->location.x + this->location.y * this->location.y);
}


Vector3 Matter::calculateGravity3D(Vector3 loc)
{
	Vector3 sepVec = this->location - loc;
	float d = loc.getDistance(this->location, loc);
	return sepVec * (G_EARTH / pow(d, 3));

}

Vector3 Matter::calculateJ2Gravity(Vector3 loc)
{
	Vector3 sepVec = this->location - loc;
	float d = loc.getDistance(this->location, loc);
	float xAcc = (sepVec.x * G_EARTH / pow(d, 3)) * (1 + (1.5 * J2) * pow(1 / d, 2) * (1 - 5 * pow(sepVec.z / d, 2)));
	float yAcc = (sepVec.y * G_EARTH / pow(d, 3)) * (1 + (1.5 * J2) * pow(1 / d, 2) * (1 - 5 * pow(sepVec.z / d, 2)));
	float zAcc = (sepVec.z * G_EARTH / pow(d, 3)) * (1 + (1.5 * J2) * pow(1 / d, 2) * (3 - 5 * pow(sepVec.z / d, 2)));
	return Vector3(xAcc, yAcc, zAcc);

}


void Matter::RK4(float dt, Matter* planet, bool useJ2)
{
	//want a pointer to the gravity member function to selectively apply different versions of gravity within RK4 method
	typedef Vector3(Matter::* GravFunc)(Vector3);
	GravFunc gravity = &Matter::calculateJ2Gravity;
	if (!useJ2){
		gravity = &Matter::calculateGravity3D;
	}

	Vector3 k1v = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 k2v = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 k3v = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 k4v = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 k1 = Vector3(velocity.x, velocity.y, velocity.z);
	Vector3 step1 = (planet->*gravity)(location);
	k1v = k1v + step1;

	Vector3 k2 = k1 + k1v * (dt * 0.5f);


	//calculate at xi + (dt/2)*k1x
	Vector3 tempLoc1 = location + (k1 * 0.5f * dt); 
	Vector3 step2 = (planet->*gravity)(tempLoc1);
	k2v = k2v + step2;

	Vector3 k3 = k1 + (k2v * 0.5f * dt);


	//calculate at xi + (dt/2)*k2x
	Vector3 tempLoc2 = location + (k2 * 0.5f * dt);
	Vector3 step3 = (planet->*gravity)(tempLoc2);
	k3v = k3v + step3;

	Vector3 k4 = k1 + (k3v * dt);


	Vector3 tempLoc3 = location + (k3 * dt); 
	Vector3 step4 = (planet->*gravity)(tempLoc3);
	k4v = k4v + step4;

	Vector3 d4 = (k1 + (k2 * 2.0f) + (k3 * 2.0f) + k4) * (dt / 6);
	Vector3 dv4 = (k1v + (k2v * 2.0f) + (k3v * 2.0f) + k4v) * (dt / 6);

	location = location + d4;
	velocity = velocity + dv4;
}


Matter::~Matter()
{
}
