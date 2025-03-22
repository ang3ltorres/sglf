#pragma once
#include <pch.hpp>

namespace sglf
{
	struct Input
	{
		static bool up[3];
		static bool down[3];
		static bool left[3];
		static bool right[3];
		static glm::uvec2 mousePos;
	
		static void keyDown(WPARAM wParam);
		static void keyUp(WPARAM wParam);
	
		static void reset();
	};
}
