#pragma once
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Matter.h"
#include "Planet.h"

class Kepler : public Planet {
public:
	//orbital parameters
	float a;
	float e;
	//argument of perihelion
	float w;
	float vAtAphelion;
	float vAtPerihelion;
	float inclination;
	float longAscNode;

	float r;
	float theta;

	void updateLoc(float dt);
	Vector3 coordTransform();
	Kepler();
	Kepler(float a, float e, float theta, float w, float inclination, float longAscNode, float size, float m);

	void setPathColor(Color pathColor);

	void drawEllipse();

	~Kepler();
private:

	Color pathColor;
};