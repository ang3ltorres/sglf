#include <renderTexture.hpp>
using namespace sglf;

#include "graphics.hpp"

RenderTexture::RenderTexture(unsigned int width, unsigned int height, Camera *camera)
: Drawable::Drawable(new Texture(width, height, 1))
{
	// Camera
	internalCamera = camera ? false : true;
	this->camera = camera ? camera : new Camera(width, height);

	glCreateFramebuffers(1, &FBO);
	glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texture->id, 0);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glNamedFramebufferDrawBuffers(FBO, 1, drawBuffers);

	updateModel();
}

RenderTexture::~RenderTexture()
{
	if (internalCamera) delete camera;
	delete texture;
	glDeleteFramebuffers(1, &FBO);
}
