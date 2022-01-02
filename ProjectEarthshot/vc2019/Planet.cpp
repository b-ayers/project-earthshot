#include "Planet.h"

Planet::Planet() : Matter()
{
	axisRotationRate = 0.2f;
	rotationAxis = vec3(0, 1, 0);
	emissiveColor = vec4(0, 0, 0, 0);
}

Planet::~Planet() 
{
}

void Planet::loadAssets(string textureName)
{
	texture = AssetLoader::loadTexture(textureName);
}

void Planet::setRotationAxis(vec3 axis)
{
	this->rotationAxis = axis;
}

void Planet::setEmissiveColor(vec4 color)
{
	this->emissiveColor = color;
}

void Planet::update()
{
	axisRotation *= glm::rotate(toRadians(axisRotationRate), normalize(rotationAxis));
}

void Planet::draw(BatchRef batchRef, float cameraZoom, float frameAlpha)
{
	gl::pushModelMatrix();

	Vector3 extrapolatedMovement = velocity * frameAlpha;
	Vector3 drawLocation = location + extrapolatedMovement;
	gl::translate(drawLocation.x, drawLocation.y, drawLocation.z);
	
	gl::scale(radius, radius, radius);
	gl::rotate(vec3(toRadians(90.0f), 0, 0));
	gl::multModelMatrix(axisRotation);
	
	texture->bind();
	
	batchRef->getGlslProg()->uniform("uEmissiveColor", emissiveColor);
	batchRef->getGlslProg()->uniform("uSunlightPosition", vec3(0, 0, 0));

	batchRef->draw();

	gl::popModelMatrix();

}

vec3 Planet::getLocation()
{
	return this->location.toVec3();
}

float Planet::getHeading() {
	return 0.0f;
}