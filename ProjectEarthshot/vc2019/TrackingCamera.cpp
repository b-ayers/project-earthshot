#include "TrackingCamera.h"
#include "CameraTrackable.h"

TrackingCamera::TrackingCamera(CameraTrackable* objectToFollow, vec3 offset, float shipScale)
{ 
	this->objectToFollow = objectToFollow;
	this->offset = offset;
	this->offsetAngle = atan2(offset.x, offset.y);
	this->offsetMagnitude = sqrt(pow(offset.x, 2) + pow(offset.y, 2));
	this->shipScale = shipScale;
}

vec3 TrackingCamera::getCameraLocation(float zoom)
{
	float x = cos(objectToFollow->getHeading() + offsetAngle) * offsetMagnitude * zoom + objectToFollow->getLocation().x;
	float y = sin(objectToFollow->getHeading() + offsetAngle) * offsetMagnitude * zoom + objectToFollow->getLocation().y;
	float z = offset.z * zoom;
	return vec3(x, y, z);
}

vec3 TrackingCamera::getLookAt()
{
	return objectToFollow->getLocation();
}

float TrackingCamera::getShipScale()
{
	return shipScale;
}
