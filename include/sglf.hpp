#pragma once

#include "pch.hpp"

#include "window.hpp"
#include "graphics.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include "sound.hpp"
#include "input.hpp"

namespace sglf
{
	void initialize(int width, int height, const char *title, const char *fonts[] = nullptr);
	void finalize();
	void endFrame();
}
