#include "Ship.h"
#include "cinder/gl/gl.h"
#include "ProjectMoonshotApp.h"

using namespace std;

Ship::Ship()
{
	this->batchRef = NULL;
	this->velocity = Vector3(0.0f, 0.0f, 0.0f);
	this->thrustAmt = 0.0f;
	this->rotationAmt = 0.0f;
	this->polarAmt = 0.0f;
	this->headingVec = Vector3(cos(polarRotation) * cos(headingRotation), cos(polarRotation) * sin(headingRotation), sin(polarRotation));
	headingVec = Vector3(cos(headingRotation), sin(headingRotation), 0.0);
	originVec = Vector3(1.0f,0.0f,0.0f);
}

void Ship::loadAssets(GlslProgRef defaultProgRef, GlslProgRef additiveProgRef) {
	this->batchRef = AssetLoader::loadObj("ship/ship_02a.obj", defaultProgRef);
	this->textureRef = AssetLoader::loadTexture("ship/ship_02_texture_01.png");
	this->flameBatchRef = AssetLoader::loadObj("ship/ship_flame.obj", additiveProgRef);
	this->flameTextureRef = AssetLoader::loadTexture("ship/flame_texture_01.png");
}

void Ship::handleKeyDown(KeyEvent event) {
	switch (event.getCode()) 
	{
		case KeyEvent::KEY_LEFT:
			rotationAmt = -0.8f;
			break;

		case KeyEvent::KEY_RIGHT:
			rotationAmt = 0.8f;
			break;

		case KeyEvent::KEY_SPACE:
			thrustAmt = 0.005f;
			break;

		case KeyEvent::KEY_DOWN:
			polarAmt = 0.8f;
			break;
		case KeyEvent::KEY_UP:
			polarAmt = -0.8f;
			break;
	}
}

void Ship::handleKeyUp(KeyEvent event) {
	switch (event.getCode()) 
	{
		case KeyEvent::KEY_LEFT:
		case KeyEvent::KEY_RIGHT:
			rotationAmt = 0.0f;
			break;
		case KeyEvent::KEY_DOWN:
		case KeyEvent::KEY_UP:
			polarAmt = 0.0f;
			break;


		case KeyEvent::KEY_SPACE:
			thrustAmt = 0.0f;
			break;
	}
}

void Ship::update(float deltaTime)
{
	rotate(deltaTime * CONTROL_SCALAR);
	thrust(deltaTime * CONTROL_SCALAR);
}

void Ship::draw(float frameAlpha)
{
	gl::pushModelMatrix();
	textureRef->bind();
	Vector3 extrapolatedMovement = velocity * frameAlpha;
	Vector3 drawLocation = location + extrapolatedMovement;
	gl::translate(drawLocation.x, drawLocation.y, drawLocation.z);
	gl::rotate(headingRotation);
	gl::rotate(-polarRotation, vec3(0.0,1.0,0.0));
	gl::scale(this->radius, this->radius, this->radius);
	batchRef->draw();

	gl::popModelMatrix();

}

void Ship::draw(float elapsedSeconds, float frameAlpha, float scale)
{
	gl::pushModelMatrix();
	textureRef->bind();
	Vector3 extrapolatedMovement = velocity * frameAlpha;
	Vector3 drawLocation = location + extrapolatedMovement;
	gl::translate(drawLocation.x, drawLocation.y, drawLocation.z);
	gl::rotate(headingRotation);
	gl::rotate(-polarRotation, vec3(0.0, 1.0, 0.0));
	gl::scale(this->radius * scale + 0.04f, this->radius * scale  + 0.04f , this->radius * scale + 0.04f);
	batchRef->draw();


	if (thrustAmt > 0) 
	{
		drawFlame(elapsedSeconds);
	}

	gl::popModelMatrix();
}

void Ship::drawThrustVector() {
	gl::drawLine(location.toVec3(), location.toVec3() + thrustVec.toVec3() * 100.0f);
}

void Ship::drawHeading() {
	gl::drawLine(location.toVec3(), location.toVec3() + headingVec.toVec3()*0.1f);

}

void Ship::drawFlame(float elapsedSeconds)
{
	flameTextureRef->bind();
	flameBatchRef->getGlslProg()->uniform("uTime", elapsedSeconds);
	flameBatchRef->getGlslProg()->uniform("uIntensity", 2.0f);
	flameBatchRef->draw();
}

void Ship::rotate(float deltaTime)
{
	headingRotation += rotationAmt * deltaTime;
	polarRotation += polarAmt * deltaTime;
	//keep polar coordinate within coordinate system range (origin at equator)
	if (polarRotation >= M_PI/2.0) {
		polarRotation = M_PI / 2.0;
	}
	else if (polarRotation <= -M_PI / 2.0){
		polarRotation = -M_PI / 2.0;
	}
	//update the heading vector of the ship
	headingVec = Vector3(cos(polarRotation)*cos(headingRotation), cos(polarRotation)*sin(headingRotation), sin(polarRotation));
	
}

void Ship::thrust(float deltaTime) {
	float dx = cos(polarRotation) * cos(headingRotation) * thrustAmt;
	float dy = cos(polarRotation) * sin(headingRotation) * thrustAmt;
	float dz = sin(polarRotation) * thrustAmt;
	thrustVec = Vector3(dx, dy, dz);
	velocity = velocity + thrustVec * deltaTime;
}

float Ship::getHeading()
{
	return headingRotation;
}

Vector3 Ship::getHeadingVec()
{
	return headingVec;
}

vec3 Ship::getLocation()
{
	return location.toVec3();
}
