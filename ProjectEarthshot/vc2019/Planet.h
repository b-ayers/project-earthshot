#pragma once

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Matter.h"
#include "AssetLoader.h"
#include "CameraTrackable.h"

using namespace ci;
using namespace ci::gl;
using namespace std;

class Planet : public Matter, public CameraTrackable
{
public:

	Planet();

	~Planet();

	void loadAssets(string textureName);

	void setRotationAxis(vec3 axis);

	void setEmissiveColor(vec4 color);

	void update();

	void draw(BatchRef batchRef, float cameraZoom, float frameAlpha);

	vec3 getLocation() override;
	
	float getHeading() override;

private:

	gl::Texture2dRef texture;

	float axisRotationRate = 0.2f;

	mat4 axisRotation;

	vec3 rotationAxis;

	vec4 emissiveColor;
};

