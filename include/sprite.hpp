#pragma once
#include <pch.hpp>

#include "drawable.hpp"
#include "texture.hpp"

namespace sglf
{
	class Sprite : public Drawable
	{
	public:
		Sprite(const Sprite&) = default;
		Sprite(Texture *texture, glm::ivec4 src, glm::ivec4 dst);
		Sprite(Texture *texture, glm::ivec4 dst);
		Sprite(Texture *texture);
		~Sprite() = default;
	};
}
