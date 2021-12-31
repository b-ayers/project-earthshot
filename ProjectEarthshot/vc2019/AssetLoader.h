#pragma once
#include "AssetLoader.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/ObjLoader.h"

using namespace ci::gl;
using namespace std;

class AssetLoader
{
public:
	
	static BatchRef loadObj(string objFile, GlslProgRef glslProg);

	static TextureRef loadTexture(string textureFile);
};

