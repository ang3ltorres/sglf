#pragma once
#include <pch.hpp>

#include "texture.hpp"
#include "color.hpp"

namespace sglf
{
	class Drawable
	{
	public:
		Drawable(const Drawable&) = delete;
		Drawable(Texture *texture);
		~Drawable() = default;
	
		Texture *texture;
		
		glm::ivec4 src;
		glm::ivec4 dst;
		Color color;
		float rotation;
	
		void updateModel();
		void batch();
	
		glm::mat4 model;
	};
}
