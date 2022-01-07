#include "Kepler.h"
#include <math.h>

Kepler::Kepler() : Planet() {
	this->r = 0;
	this->theta = 0;
	this->a = 0;
	this->e = 0;
	this->w = 0;
	this->longAscNode = 0;
	this->inclination = 0;
	this->vAtAphelion = 0;
	this->vAtPerihelion = 0;
}

Kepler::Kepler(float a, float e, float theta, float w, float inclination, float longAscNode, float size, float m) : Planet() {
	this->a = a;
	this->e = e;
	this->w = w;
	this->theta = theta;
	this->r = a * (1 - e * e) / (1 + e * cos(theta+w));
	this->vAtAphelion = sqrt((float(G_EARTH) / a) * (1 - e * e)) / (1 + e);
	this->vAtPerihelion = sqrt((float(G_EARTH) / a) * (1 - e * e)) / (1 - e);
	this->location = Vector3(r * cos(theta), r * sin(theta), 0);
	this->longAscNode = longAscNode;
	this->inclination = inclination;
	this->radius = size;
	this->mass = m;

}

void Kepler::updateLoc(float dt) {
	float dtheta = dt * sqrt((1 - e * e) * float(G_EARTH) * a) / (r * r);
	theta = theta + dtheta;
	r = a * (1 - e * e) / (1 + e * cos(theta+w));
	location = coordTransform();

}
//transform to the global coordinates from the coordinate system in the plane of the orbit
Vector3 Kepler::coordTransform()
{
	float c = cos(w + theta);
	float s = sin(w + theta);
	float x2 = r * (cos(longAscNode) * c - sin(longAscNode) * s * cos(inclination));
	float y2 = r * (sin(longAscNode) * c + cos(longAscNode) * s * cos(inclination));
	float z2 = r * s * sin(inclination);
	return Vector3(x2, y2, z2);
}



void Kepler::setPathColor(Color pathColor)
{
	this->pathColor = pathColor;
}

void Kepler::drawEllipse()
{
	gl::pushModelMatrix();
	gl::rotate(longAscNode);
	gl::rotate(inclination, vec3(1, 0, 0));
	gl::rotate(w);
	gl::color(pathColor);
	gl::drawStrokedEllipse(vec2(0.0f, 0.0f), a, a * sqrt(1 - e * e), 300.0f);
	gl::color(Color(1, 1, 1));
	gl::popModelMatrix();
}

Kepler::~Kepler() {
}