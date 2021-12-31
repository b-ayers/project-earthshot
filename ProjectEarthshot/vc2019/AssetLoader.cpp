#include "AssetLoader.h"
#include "cinder/ObjLoader.h"

using namespace ci::app;
using namespace ci::gl;
using namespace ci;


BatchRef AssetLoader::loadObj(string objFile, GlslProgRef glslProg)
{
	ObjLoader loader(loadAsset(objFile));
	TriMeshRef mMesh = TriMesh::create(loader);

	if (!loader.getAvailableAttribs().count(geom::NORMAL))
		mMesh->recalculateNormals();

	return gl::Batch::create(*mMesh, glslProg);
}

TextureRef AssetLoader::loadTexture(string textureFile)
{
	return gl::Texture::create(loadImage(loadAsset(textureFile)), gl::Texture::Format().mipmap());
}
