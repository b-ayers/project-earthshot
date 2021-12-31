#pragma once
#include "Matter.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "AssetLoader.h"
#include "CameraTrackable.h"

using namespace ci::app;
using namespace ci;
using namespace std;

class Ship : public Matter, public CameraTrackable
{
public:

	const float CONTROL_SCALAR = 1.0f;

	Ship();

	void loadAssets(GlslProgRef defaultGlProg, GlslProgRef additiveGlProg);

	void handleKeyUp(KeyEvent event);
	void handleKeyDown(KeyEvent event);

	void update(float deltaTime);

	void draw(float frameAlpha);
	void draw(float elapsedSeconds, float frameAlpha, float scale);

	void drawThrustVector();

	void drawHeading();

	float getHeading() override;
	Vector3 getHeadingVec();

	vec3 getLocation() override;

private:

	void drawFlame(float elapsedSeconds);
	void rotate(float scale);
	void thrust(float scale);

	float rotationAmt = 1.0f;
	float polarAmt = 0.0f;
	float thrustAmt = 0.0f;

	float headingRotation = 0.0f;
	float polarRotation = 0.0f;

	Vector3 headingVec;
	Vector3 originVec;
	Vector3 thrustVec;

	gl::BatchRef batchRef;
	gl::Texture2dRef textureRef;
	gl::BatchRef flameBatchRef;
	gl::Texture2dRef flameTextureRef;
};

