#include <sglf.hpp>
using namespace sglf;
using namespace glm;

#include "window.hpp"
#include "graphics.hpp"
#include "sound.hpp"
#include "input.hpp"

void sglf::initialize(int width, int height, const char *title, const char *fonts[])
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	Window::initialize(width, height, title);
	Graphics::initialize();
	Texture::initialize();
	Sound::initialize();
	Font::initialize(fonts);
}

void sglf::finalize()
{
	Font::finalize();
	Sound::finalize();
	Texture::finalize();
	Graphics::finalize();
	Window::finalize();
	CoUninitialize();
}

void sglf::endFrame()
{
	SwapBuffers(Graphics::dc);
	Input::reset();
	Graphics::updateTime();
}
