#pragma once
#include "cinder/gl/gl.h"

class Vector3
{

	public: 

		Vector3();

		Vector3(float x, float y, float z);

		float x;
		float y;
		float z;

		glm::vec3 toVec3();

		static float getDistance(Vector3 p1, Vector3 p2);
		static float getDistanceSquared(Vector3 p1, Vector3 p2);
		float getMagnitude();
		Vector3 operator+(Vector3 &v2);
		Vector3 operator-(Vector3 &v2);
		Vector3 operator*(float f);
		Vector3 operator*(Vector3& v);

};

