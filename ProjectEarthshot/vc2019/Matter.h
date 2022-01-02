#pragma once
#include "Vector3.h"
#include <vector>

const float M_PER_AU = 1.49598e11;
const float SOLAR_MASS_KG = 1.989e30;
const float SECONDS_IN_DAY = 86400;
static const float G = -(6.673e-11*SOLAR_MASS_KG*SECONDS_IN_DAY*SECONDS_IN_DAY/(M_PER_AU*M_PER_AU*M_PER_AU));
const double EARTH_MASS_KG = 5.972e24;
const float G_CONST_SI = 6.67430e-11;
const double EARTH_RAD_EQ_M = 6378137.0;
const double J2 = 1.0826e-3;
const float MU_SI = 398600.0e9;
const float MU_EUNITS = 398600.0e9 * SECONDS_IN_DAY * SECONDS_IN_DAY / (EARTH_RAD_EQ_M * EARTH_RAD_EQ_M * EARTH_RAD_EQ_M);
const double G_EARTH = (6.673e-11 * EARTH_MASS_KG * 60.0 * 60.0 / (EARTH_RAD_EQ_M * EARTH_RAD_EQ_M * EARTH_RAD_EQ_M)); //length units of earth radius, time units minutes


class Matter
{

public:

	Matter();
	~Matter();

	float mass;
	float radius;

	Vector3 location;
	Vector3 velocity;

	//template <typename T> void applyGravity(float deltaTime, std::vector<T> bodies) {}
	float getDist(Matter);
	Vector3 getSep(Matter);
	void updatePos(float dt);
	float Matter::getSpeedSquared();
	void Matter::RK4(float dt, Matter* planet, bool useJ2);
	virtual Vector3 calculateGravity3D(Vector3 loc);
	Vector3 calculateJ2Gravity(Vector3 loc);



private:
	
	
};

