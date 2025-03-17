#include <input.hpp>
using namespace sglf;

#include "window.hpp"

bool Input::up[3];
bool Input::down[3];
bool Input::left[3];
bool Input::right[3];
glm::uvec2 Input::mousePos;

void Input::keyDown(WPARAM wParam)
{
	switch (wParam)
	{
		case VK_ESCAPE:
			Window::forceClose = true;
			return;

		case VK_UP   : (!Input::up[0])    ? (Input::up[0] = Input::up[1] = true)      : Input::up[0];    break;
		case VK_DOWN : (!Input::down[0])  ? (Input::down[0] = Input::down[1] = true)  : Input::down[0];  break;
		case VK_LEFT : (!Input::left[0])  ? (Input::left[0] = Input::left[1] = true)  : Input::left[0];  break;
		case VK_RIGHT: (!Input::right[0]) ? (Input::right[0] = Input::right[1] = true): Input::right[0]; break;
	}
}

void Input::keyUp(WPARAM wParam)
{
	switch (wParam)
	{
		case VK_UP:    Input::up[0] = false;    Input::up[2]    = true; break;
		case VK_DOWN:  Input::down[0] = false;  Input::down[2]  = true; break;
		case VK_LEFT:  Input::left[0] = false;  Input::left[2]  = true; break;
		case VK_RIGHT: Input::right[0] = false; Input::right[2] = true; break;
	}
}

void Input::reset()
{
	Input::up[1]    = false;
	Input::down[1]  = false;
	Input::left[1]  = false;
	Input::right[1] = false;

	Input::up[2]    = false;
	Input::down[2]  = false;
	Input::left[2]  = false;
	Input::right[2] = false;
}