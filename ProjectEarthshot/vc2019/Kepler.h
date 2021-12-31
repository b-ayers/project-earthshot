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
	static void Kepler::heun(float dt, std::vector<Kepler*> planets, Matter* s);
	static Vector3 Kepler::adaptiveRK45(float dt, std::vector <Kepler*> planets, Matter* s, float tolerance);

	void setPathColor(Color pathColor);

	void drawEllipse();

	~Kepler();
private:

	Color pathColor;
};