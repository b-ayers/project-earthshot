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
	float w;
	float vAtAphelion;
	float vAtPerihelion;

	float r;
	float theta;

	void updateLoc(float dt);
	Kepler();
	Kepler(float a, float e, float theta, float w, float size, float m);

	void setPathColor(Color pathColor);

	void drawEllipse();

	~Kepler();
private:

	Color pathColor;
};