#pragma once
#include <pch.hpp>

#include "drawable.hpp"
#include "camera.hpp"

namespace sglf
{
	struct RenderTexture : public Drawable
	{
		RenderTexture(unsigned int width, unsigned int height, Camera *camera = nullptr);
		RenderTexture(const RenderTexture&) = delete;
		~RenderTexture();
	
		GLuint FBO;
	
		bool internalCamera;
		Camera *camera;
	};
}
