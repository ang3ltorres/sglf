#pragma once
#include <pch.hpp>

#include "drawable.hpp"
#include "font.hpp"
#include "color.hpp"

namespace sglf
{
	struct Text : public Drawable
	{
		Text(const char *text, Font *font, glm::vec2 pos, Color color = {0, 0, 0, 255});
		Text(const Text&) = delete;
		~Text();
	
		void setText(const char *text);
		void render();
	
		char *text;
		Font *font;
	};
}
