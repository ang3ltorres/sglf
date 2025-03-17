#pragma once
#include <pch.hpp>

#include "drawable.hpp"
#include "camera.hpp"

namespace sglf
{
	class RenderTexture : public Drawable
	{
	public:
		RenderTexture(unsigned int width, unsigned int height, Camera *camera = nullptr);
		RenderTexture(const RenderTexture&) = delete;
		~RenderTexture();
	
		GLuint FBO;
	
		bool internalCamera;
		Camera *camera;
	};
}
