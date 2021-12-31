#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/app/MouseEvent.h"
#include "cinder/gl/gl.h"
#include "Matter.h"
#include "Planet.h"
#include "Ship.h"
#include "Kepler.h"
#include "TrackingCamera.h"
#include "cinder/CameraUi.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ProjectMoonshotApp : public App 
{

public:

	const float SECONDS_PER_UPDATE = 0.01f;
	const float GAME_TIME_SCALE = 4.0f;
	const float FRAME_RATE_LIMIT = 60.0f;
	float timeFraction = 1.0f;
	//adaptable stepsize, initialized to seconds per update
	float stepSize = 0.003f;

	void setup() override;
	void keyUp(KeyEvent event) override;
	void keyDown(KeyEvent event) override;
	void mouseDown(MouseEvent event) override;
	void mouseDrag(MouseEvent event) override;
	void update() override;
	void constantUpdate(float deltaTime);
	void updateGui();
	void resize() override;
	void draw() override;

	void createPlanets();
	void createShip();

	ivec2 mLastMousePos;
	CameraPersp	mCam;
	CameraUi mCamUi;
	CameraOrtho camOrtho;
	gl::BatchRef mBatch;
	gl::BatchRef sunBatch;
	gl::TextureRef mTexture;
	gl::GlslProgRef defaultGlProg;
	gl::GlslProgRef sunGlProg;
	gl::GlslProgRef additiveGlProg;
	mat4 mCubeRotation;

	float lastUpdateTime = 0.0f;
	float lastDrawTime = 0.0f;
	float updateAccumulator = 0.0f;

	float frameRate = 0.0f;
	float updateRate = 0.0f;

	//vector<TrackingCamera*> cameraItems;
	float cameraZoom = 1.0f;
	int cameraSelect = 0;

	bool gravJ2 = true;

	vector<Planet*> planets;
	Planet* earth;
	Ship* ship;
		
};
