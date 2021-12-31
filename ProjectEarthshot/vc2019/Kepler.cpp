#include "Kepler.h"
#include <math.h>
const float M = 1.0f;
float GM = (-G) * M;
const float DIST_CUTOFF = 0.2f;
Kepler::Kepler() {
	Planet();
	this->r = 0;
	this->theta = 0;
	this->a = 0;
	this->e = 0;
	this->w = 0;
	this->vAtAphelion = 0;
}

Kepler::Kepler(float a, float e, float theta, float w, float size, float m) {
	Planet();
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

//execute 4th order and 5th order RK methods simultaneously, take difference to determine adaptive step size
//needs to be vectorized and optimized
//http://maths.cnam.fr/IMG/pdf/RungeKuttaFehlbergProof.pdf
Vector3 Kepler::adaptiveRK45(float dt, std::vector <Kepler*> planets, Matter* s, float tolerance) {
	float l1x = 0.0f;
	float l1y = 0.0f;
	float l2x = 0.0f;
	float l2y = 0.0f;
	float l3x = 0.0f;
	float l3y = 0.0f;
	float l4x = 0.0f;
	float l4y = 0.0f;
	float l5x = 0.0f;
	float l5y = 0.0f;
	float l6x = 0.0f;
	float l6y = 0.0f;



	float k1x = s->velocity.x;
	float k1y = s->velocity.y;
	for (Kepler* p : planets) {
		float dist = pow(s->getDist(*p), 3);
		float sepX = s->getSep(*p).x;
		float sepY = s->getSep(*p).y;
		l1x += G * p->mass * sepX / dist;
		l1y += G * p->mass * sepY / dist;

	}
	float sunDist1 = pow(s->location.getMagnitude(), 3);
	l1x = l1x + (G * M * s->location.x / sunDist1);
	l1y = l1y + (G * M * s->location.y / sunDist1);





	float k2x = k1x + l1x * dt * 0.25f;
	float k2y = k1y + l1y * dt * 0.25f;
	Vector3 tempLoc1 = s->location + Vector3(dt * 0.25f * k1x, dt * 0.25f * k1y, 0);
	for (Kepler* p : planets) {
		float dist = pow(Vector3::getDistance(tempLoc1, p->location), 3);
		float sepX = (tempLoc1 - p->location).x;
		float sepY = (tempLoc1 - p->location).y;
		l2x += G * p->mass * sepX / dist;
		l2y += G * p->mass * sepY / dist;
	}
	float sunDist2 = pow(tempLoc1.getMagnitude(), 3);
	l2x = l2x + (G * M * tempLoc1.x / sunDist2);
	l2y = l2y + (G * M * tempLoc1.y / sunDist2);


	float a3 = 3.0f / 32.0f;
	float b3 = 9.0f / 32.0f;

	float k3x = k1x + a3 * dt * l1x + b3 * dt * l2x;
	float k3y = k1y + a3 * dt * l1y + b3 * dt * l2y;
	Vector3 tempLoc2 = s->location + Vector3(a3 * dt * k1x + b3 * dt * k2x, a3 * dt * k1y + b3 * dt * k2y, 0);
	for (Kepler* p : planets) {
		float dist = pow(Vector3::getDistance(tempLoc2, p->location), 3);
		float sepX = (tempLoc2 - p->location).x;
		float sepY = (tempLoc2 - p->location).y;
		l3x += G * p->mass * sepX / dist;
		l3y += G * p->mass * sepY / dist;
	}
	float sunDist3 = pow(tempLoc2.getMagnitude(), 3);
	l3x = l3x + (G * M * tempLoc2.x / sunDist3);
	l3y = l3y + (G * M * tempLoc2.y / sunDist3);



	float a4 = 1932.0f / 2197.0f;
	float b4 = 7200.0f / 2197.0f;
	float c4 = 7296.0f / 2197.0f;

	float k4x = k1x + a4 * dt * l1x - b4 * dt * l2x + c4 * dt * l3x;
	float k4y = k1y + a4 * dt * l1y - b4 * dt * l2y + c4 * dt * l3y;
	Vector3 tempLoc3 = s->location + Vector3(a4 * dt * k1x - b4 * dt * k2x + c4 * dt * k3x,
		a4 * dt * k1y - b4 * dt * k2y + c4 * dt * k3y, 0);
	for (Kepler* p : planets) {
		float dist = pow(Vector3::getDistance(tempLoc3, p->location), 3);
		float sepX = (tempLoc3 - p->location).x;
		float sepY = (tempLoc3 - p->location).y;
		l4x += G * p->mass * sepX / dist;
		l4y += G * p->mass * sepY / dist;
	}
	float sunDist4 = pow(tempLoc3.getMagnitude(), 3);
	l4x = l4x + (G * M * tempLoc3.x / sunDist4);
	l4y = l4y + (G * M * tempLoc3.y / sunDist4);



	float a5 = (439.0f / 216.0f);
	float b5 = 8.0f;
	float c5 = (3680.0f / 513.0f);
	float d5 = (845.0f / 4104.0f);

	float k5x = k1x + a5 * dt * l1x - b5 * dt * l2x + c5 * dt * l3x - d5 * dt * l4x;
	float k5y = k1y + a5 * dt * l1y - b5 * dt * l2y + c5 * dt * l3y - d5 * dt * l4y;
	Vector3 tempLoc4 = s->location + Vector3(a5 * dt * k1x - b5 * dt * k2x + c5 * dt * k3x - d5 * dt * k4x,
		a5 * dt * k1y - b5 * dt * k2y + c5 * dt * k3y - d5 * dt * k4y, 0);
	for (Kepler* p : planets) {
		float dist = pow(Vector3::getDistance(tempLoc4, p->location), 3);
		float sepX = (tempLoc4 - p->location).x;
		float sepY = (tempLoc4 - p->location).y;
		l5x += G * p->mass * sepX / dist;
		l5y += G * p->mass * sepY / dist;
	}
	float sunDist5 = pow(tempLoc4.getMagnitude(), 3);
	l5x = l5x + (G * M * tempLoc4.x / sunDist5);
	l5y = l5y + (G * M * tempLoc4.y / sunDist5);


	float a6 = (8.0f / 27.0f);
	float b6 = 2.0f;
	float c6 = (3544.0f / 2565.0f);
	float d6 = (1859.0f / 4104.0f);
	float e6 = (11.0f / 40.0f);


	float k6x = k1x - a6 * dt * l1x + b6 * dt * l2x - c6 * dt * l3x + d6 * dt * l4x - e6 * dt * l5x;
	float k6y = k1y - a6 * dt * l1y + b6 * dt * l2y - c6 * dt * l3y + d6 * dt * l4y - e6 * dt * l5y;
	Vector3 tempLoc5 = s->location + Vector3(-a6 * dt * k1x + b6 * dt * k2x - c6 * dt * k3x + d6 * dt * k4x - e6 * dt * k5x,
		-a6 * dt * k1y + b6 * dt * k2y - c6 * dt * k3y + d6 * dt * k4y - e6 * dt * k5y, 0);
	for (Kepler* p : planets) {
		float dist = pow(Vector3::getDistance(tempLoc5, p->location), 3);
		float sepX = (tempLoc5 - p->location).x;
		float sepY = (tempLoc5 - p->location).y;
		l6x += G * p->mass * sepX / dist;
		l6y += G * p->mass * sepY / dist;
	}
	float sunDist6 = pow(tempLoc5.getMagnitude(), 3);
	l6x = l6x + (G * M * tempLoc5.x / sunDist6);
	l6y = l6y + (G * M * tempLoc5.y / sunDist6);

	float a7 = (25.0f / 216.0f);
	float b7 = (1408.0f / 2565.0f);
	float c7 = (2197.0f / 4101.0f);
	float d7 = 0.2f;

	float dx4 = dt * (a7 * k1x + b7 * k3x + c7 * k4x - d7 * k5x);
	float dy4 = dt * (a7 * k1y + b7 * k3y + c7 * k4y - d7 * k5y);
	float dvx4 = dt * (a7 * l1x + b7 * l3x + c7 * l4x - d7 * l5x);
	float dvy4 = dt * (a7 * l1y + b7 * l3y + c7 * l4y - d7 * l5y);

	float a8 = (16.0f / 135.0f);
	float b8 = (6656.0f / 12825.0f);
	float c8 = (28561.0f / 56430.0f);
	float d8 = (9.0f / 50.0f);
	float e8 = (2.0f / 55.0f);

	float dx5 = dt * (a8 * k1x + b8 * k3x + c8 * k4x - d8 * k5x + e8 * k6x);
	float dy5 = dt * (a8 * k1y + b8 * k3y + c8 * k4y - d8 * k5y + e8 * k6y);
	float dvx5 = dt * (a8 * l1x + b8 * l3x + c8 * l4x - d8 * l5x + e8 * l6x);
	float dvy5 = dt * (a8 * l1y + b8 * l3y + c8 * l4y - d8 * l5y + e8 * l6y);

	//s->location.x = s->location.x + dx5;
	//s->location.y = s->location.y + dy5;
	//s->velocity.x = s->velocity.x + dvx5;
	//s->velocity.y = s->velocity.y + dvy5;
	Vector3 newLoc = Vector3(dx5, dy5, 0);
	return newLoc;
	//float diffX = abs(dx5 - dx4);
	//float diffY = abs(dy5 - dy4);
	//float diff = sqrt((dx5 - dx4) * (dx5 - dx4) + (dy5 - dy4) * (dy5 - dy4));
	//float diff = sqrt(dx5 * dx5 + dy5 * dy5);
	//return diff;
	//float q = 0.84f * pow((tolerance / diff), 0.25f);
	//return (q * dt);
	

}



// basically a second order runge kutta method
void Kepler::heun(float dt, std::vector<Kepler*> planets, Matter* s) {
	float k1vx = 0.0f;
	float k1vy = 0.0f;
	float k2vx = 0.0f;
	float k2vy = 0.0f;
	float k1x = s->velocity.x;
	float k1y = s->velocity.y;

	for (Kepler* p : planets) {
		float sepX = s->getSep(*p).x;
		float sepY = s->getSep(*p).y;
		float dist = pow(s->getDist(*p), 3);
		k1vx += G * p->mass * sepX / dist;
		k1vy += G * p->mass * sepY / dist;
	}

	float sunDist1 = pow(s->location.getMagnitude(), 3);
	k1vx = k1vx + (G * M * s->location.x / sunDist1);
	k1vy = k1vy + (G * M * s->location.y / sunDist1);

	float k2x = k1x + k1vx * dt;
	float k2y = k1y + k1vy * dt;

	for (Kepler* p : planets) {
		p->updateLoc(dt);
	}

	Vector3 tempLoc1 = s->location + Vector3(dt * k1x, dt * k1y, 0);
	for (Kepler* p : planets) {
		float sepX = (tempLoc1 - p->location).x;
		float sepY = (tempLoc1 - p->location).y;
		float dist = pow(Vector3::getDistance(tempLoc1, p->location), 3);
		k2vx += G * p->mass * sepX / dist;
		k2vy += G * p->mass * sepY / dist;
	}
	float sunDist2 = pow(tempLoc1.getMagnitude(), 3);
	k2vx = k2vx + (G * M * tempLoc1.x / sunDist2);
	k2vy = k2vy + (G * M * tempLoc1.y / sunDist2);

	s->location.x = s->location.x + dt * 0.5f * (k1x + k2x);
	s->location.y = s->location.y + dt * 0.5f * (k1y + k2y);
	s->velocity.x = s->velocity.x + dt * 0.5f * (k1vx + k2vx);
	s->velocity.y = s->velocity.y + dt * 0.5f * (k1vy + k2vy);
	
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