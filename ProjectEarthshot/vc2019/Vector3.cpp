#include "Vector3.h"
#include <math.h>
#include "cinder/gl/gl.h"

Vector3::Vector3()
{

}

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

glm::vec3 Vector3::toVec3()
{
	return glm::vec3(x, y, z);
}

float Vector3::getDistance(Vector3 p1, Vector3 p2) {
	float distSquared = 
		(p2.x - p1.x) * (p2.x - p1.x)
		+ (p2.y - p1.y) * (p2.y - p1.y)
		+ (p2.z - p1.z) * (p2.z - p1.z);
	return sqrt(distSquared);
}

float Vector3::getDistanceSquared(Vector3 p1, Vector3 p2) {
	float distSquared =
		(p2.x - p1.x) * (p2.x - p1.x)
		+ (p2.y - p1.y) * (p2.y - p1.y)
		+ (p2.z - p1.z) * (p2.z - p1.z);
	return distSquared;
}

float Vector3::getMagnitude() {
	float mag = (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
	return sqrt(mag);
}

Vector3 Vector3::operator+(Vector3& v2)
{
	return Vector3(v2.x + x, v2.y + y, v2.z + z);
}

Vector3 Vector3::operator-(Vector3& v2) 
{
	return Vector3(x - v2.x, y - v2.y, z - v2.z);
}

Vector3 Vector3::operator*(float f)
{
	return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator*(Vector3 &v)
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}