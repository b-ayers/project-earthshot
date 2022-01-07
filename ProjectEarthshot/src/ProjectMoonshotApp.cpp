
#include "../vc2019/ProjectMoonshotApp.h"
#include "cinder/Rand.h"
#include "cinder/CameraUi.h"
#include "cinder/CinderImGui.h"
#include "cinder/Log.h"
#include "cinder/Camera.h"
#include "cinder/app/MouseEvent.h"
#include <algorithm>

using namespace ci;
using namespace ci::app;
using namespace std;

void ProjectMoonshotApp::setup()
{
	//initialize camera
	mCam.lookAt(vec3(6, 0, 0), vec3(0),vec3(0,0,1));
	mCamUi = CameraUi(&mCam);

	try {
		mTexture = gl::Texture::create(loadImage(loadAsset("texture.jpg")), gl::Texture::Format().mipmap());
		printf("Loaded texture");
	}
	catch (const std::exception & e) {
		printf("Texture Error: %s", e.what());
	}

	try {
#if defined( CINDER_GL_ES )
		mGlsl = gl::GlslProg::create(loadAsset("shader_es2.vert"), loadAsset("shader_es2.frag"));
#else
		defaultGlProg = gl::GlslProg::create(loadAsset("shaders/shadow_shader.vert"), loadAsset("shaders/shadow_shader.frag"));
		sunGlProg = gl::GlslProg::create(loadAsset("shaders/sun_shader.vert"), loadAsset("shaders/sun_shader.frag"));
		additiveGlProg = gl::GlslProg::create(loadAsset("shaders/additive_shader.vert"), loadAsset("shaders/additive_shader.frag"));
#endif
		printf("Loaded shader");
	}
	catch (const ci::gl::GlslProgCompileExc & e) {
		CI_LOG_EXCEPTION("Error compiling:", e);
	}

	try {
		mBatch = gl::Batch::create(geom::Sphere().subdivisions(64), defaultGlProg);
		sunBatch = gl::Batch::create(geom::Sphere().subdivisions(64), sunGlProg);
		printf("Created batch");
	}
	catch (const cinder::gl::GlslProgCompileExc & e) {
		printf("Shader Error: %s", e.what());
	}

	gl::enableDepthWrite();
	gl::enableDepthRead();

	lastUpdateTime = getElapsedSeconds();

	disableFrameRate();

	createPlanets();
	createShip();

	ImGui::Initialize();
}


void::ProjectMoonshotApp::keyUp(KeyEvent event)
{

	ship->handleKeyUp(event);
}

void::ProjectMoonshotApp::keyDown(KeyEvent event)
{
	//speed up or slow down simulation time
	if (event.getChar() == 'a') {
		if (timeFraction > 0.5f) {
			timeFraction -= 0.04f;
		}
	}
	if (event.getChar() == 'd') {
		if (timeFraction < 4.0f) {
			timeFraction += 0.04f;
		}
	}
	
	//toggle between calculating J2 perturbation
	switch (event.getCode())
	{
		case KeyEvent::KEY_TAB:
			gravJ2 = !gravJ2;
			break;
	}
	ship->handleKeyDown(event);
}



void ProjectMoonshotApp::mouseDown(MouseEvent event)
{
	mCamUi.mouseDown(event);
}

void ProjectMoonshotApp::mouseDrag(MouseEvent event)
{
	mCamUi.mouseDrag(event);
}



void ProjectMoonshotApp::resize()
{
	mCam.setAspectRatio(getWindowAspectRatio());
	gl::setMatrices(mCam);
}

void ProjectMoonshotApp::update()
{
	float currentTime = getElapsedSeconds();
	float elapsedTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;

	updateAccumulator += elapsedTime;

	int updatesPerFrame = 0;
	//concept taken from here https://gameprogrammingpatterns.com/game-loop.html
	while (updateAccumulator >= SECONDS_PER_UPDATE) {
		updateAccumulator -= SECONDS_PER_UPDATE;
		constantUpdate(SECONDS_PER_UPDATE*GAME_TIME_SCALE*timeFraction);
		updatesPerFrame++;
	}
	updateRate = updatesPerFrame;
	updateGui();
}

void ProjectMoonshotApp::constantUpdate(float deltaTime)
{
	earth->update();
	//calculate gravity, use J2 perturbation if toggled
	ship->RK4(deltaTime, earth, gravJ2);
	ship->update(deltaTime);
	satellite->updateLoc(deltaTime);

}

void ProjectMoonshotApp::updateGui()
{
	ImGui::Begin("Params");
	ImVec4 color = ImVec4(0, 1, 0, 1); //green
	if (frameRate < 30) {
		color = ImVec4(1, 0, 0, 1); //red
	}
	else if (frameRate < 58) {
		color = ImVec4(1, 1, 0, 1); //yellow
	}
	ImGui::TextColored(color, "Frame Rate %.2f fps", frameRate);
	ImGui::Text("Update Rate %.4fs", updateRate);
	ImGui::Text("Step size %.4fs", stepSize/SECONDS_PER_UPDATE);
	ImGui::Text("heading %.4fs", ship->getHeadingVec().z);
	ImGui::End();
}

void ProjectMoonshotApp::draw()
{
	float frameAlpha = updateAccumulator;
	float elapsedSeconds = getElapsedSeconds();
	float deltaTime = elapsedSeconds - lastDrawTime;


	
	long millisToNextFrame = ((1.0f / FRAME_RATE_LIMIT) - deltaTime) * 1000;
	if (millisToNextFrame > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(millisToNextFrame));
	}
		
	//Apply some smoothing to the framerate so it's readable.
	frameRate = (frameRate * .95f) + (1.0 / (getElapsedSeconds() - lastDrawTime)) * .05f ;
	lastDrawTime = getElapsedSeconds();

	gl::clear();
	resize();
	
	mTexture->bind();

	////PREDICTIVE ELEMENT///////////////////

	//copy ship into temp ship to perform iterations into the future
	Matter* tShip = new Matter();
	tShip->location = ship->location;
	tShip->velocity = ship->velocity;


	float dt = SECONDS_PER_UPDATE*GAME_TIME_SCALE*timeFraction;
	float future = dt*10000/(timeFraction*2.0f);

	//toggle bool draws a line for every other future time step
	bool dotting = false;
	for (float t = 0; t < future; t = t + dt) {

		Vector3 prevLoc = tShip->location;
		//perform RK4 future time step to update temp ship's location/velocity
		tShip->RK4(dt, earth, gravJ2);
		//draw line from one time step position to next, for every other time step
		if(dotting){
			gl::drawLine(prevLoc.toVec3(), tShip->location.toVec3());
		}
		dotting = !dotting;
	}

	//draw ship and earth
	ship->draw(getElapsedSeconds(), frameAlpha, 1.0f);
	earth->draw(sunBatch, cameraZoom, frameAlpha);
	satellite->draw(mBatch, cameraZoom, frameAlpha);
	//gl::drawCube(satellite->location.toVec3(), vec3(0.1,0.1,0.1));
	satellite->drawEllipse();

	//draw some lines aligned with ship heading and thrust vector, for debugging purposes
	ship->drawThrustVector();
	ship->drawHeading();

}

void ProjectMoonshotApp::createPlanets()
{

	earth = new Planet();
	earth->location = Vector3(0, 0, 0);
	earth->velocity = Vector3(0, 0, 0);
	earth->mass = 1.0f;
	earth->radius = 1.0f; 
	earth->loadAssets("planets/earth_texture.png");
	earth->setEmissiveColor(vec4(0.0f, 0.0f, 0.12f, 1.0f));
	

}

void ProjectMoonshotApp::createShip()
{	
	ship = new Ship();
	ship->loadAssets(defaultGlProg, additiveGlProg);
	ship->location = Vector3(-1.2, 0, 0);
	ship->velocity = Vector3(0, -0.07, 0);
	ship->radius = 0.05f;
	ship->mass = 0.00f;

	//starlink-3132
	satellite = new Kepler(6733.0 / 6378.0, 0.0014, 0.0, 1.87, 0.9271, 4.309, 0.02, 0.00);
	satellite->loadAssets("planets/mars_texture.png");
	satellite->setPathColor(Color(1.0f, .4f, .4f));
	satellite->setEmissiveColor(vec4(.1f, .05f, .05f, 1.0f));



}

CINDER_APP(ProjectMoonshotApp, RendererGl(RendererGl::Options().msaa(16)))
