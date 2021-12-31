#pragma once
#include "Matter.h"
#include "CameraTrackable.h"
#include "cinder/gl/gl.h"

using namespace ci;

class TrackingCamera {

public:
    
    TrackingCamera(CameraTrackable* objectToFollow, vec3 offset, float shipScale);

    vec3 getCameraLocation(float zoom);

    vec3 getLookAt();

    float getShipScale();

private:

    CameraTrackable* objectToFollow;

    vec3 offset;

    float offsetAngle;

    float offsetMagnitude;

    float shipScale = 1.0f;
};