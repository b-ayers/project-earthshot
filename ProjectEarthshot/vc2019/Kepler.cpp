#include "Kepler.h"
#include <math.h>
const float M = 1.0f;
float GM = (-G) * M;
const float DIST_CUTOFF = 0.2f;
Kepler::Kepler() : Planet() {
	this->r = 0;
	this->theta = 0;
	this->a = 0;
	this->e = 0;
	this->w = 0;
	this->vAtAphelion = 0;
	this->vAtPerihelion = 0;
}

Kepler::Kepler(float a, float e, float theta, float w, float size, float m) : Planet() {
	this->a = a;
	this->e = e;
	this->w = w;
	this->theta = theta;
	this->r = a * (1 - e * e) / (1 + e * cos(theta+w));
	this->vAtAphelion = sqrt((GM / a) * (1 - e * e)) / (1 + e);
	this->vAtPerihelion = sqrt((GM / a) * (1 - e * e)) / (1 - e);
	this->location = Vector3(r * cos(theta), r * sin(theta), 0);
	this->radius = size;
	this->mass = m;

}

void Kepler::updateLoc(float dt) {
	float dtheta = dt* sqrt((1-e*e)*GM*a) / (r*r);
	theta = theta + dtheta;
	r = a * (1 - e * e) / (1 + e * cos(theta+w));
	location = Vector3(r * cos(theta), r * sin(theta), 0);

}



void Kepler::setPathColor(Color pathColor)
{
	this->pathColor = pathColor;
}

void Kepler::drawEllipse()
{
	gl::pushModelMatrix();

	float rotAngle = M_PI - w;
	//should add color member to class and draw ellipses in color
	gl::rotate(rotAngle);
	gl::translate((a * e), 0);
	gl::color(pathColor);
	gl::drawStrokedEllipse(vec2(0.0f, 0.0f), a, a * sqrt(1 - e * e), 300.0f);
	gl::color(Color(1, 1, 1));
	gl::popModelMatrix();
}

Kepler::~Kepler() {
}