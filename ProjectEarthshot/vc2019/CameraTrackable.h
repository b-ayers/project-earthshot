#pragma once
#include "cinder/gl/gl.h"

using namespace ci;

class CameraTrackable
{
public:

	virtual float getHeading() = 0;

	virtual vec3 getLocation() = 0;
};

