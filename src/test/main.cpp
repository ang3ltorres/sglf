#include <sglf/sglf.hpp>
#include <cstdio>

int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	unsigned int width = 800;
	unsigned int height = 600;
	
	sglf::initialize(width, height, "OpenGL", hInstance, nCmdShow);
	
	sglf::RenderTexture *renderTexture = new sglf::RenderTexture{256, 240};
	sglf::Texture *textureAtlas = new sglf::Texture{"H:/sglf_res/kirby.png"};
	sglf::Sprite *sprite = new sglf::Sprite{textureAtlas, {0, 0, 23, 22}, {0, 0, 23 * 8, 22 * 8}};
	sglf::Sound *sound = new sglf::Sound{"H:/sglf_res/coin.ogg"};

	while (!sglf::Window::shouldClose())
	{
		// Update logic
		if (sglf::Input::down[1]) sound->play();

		if (sglf::Input::up[0]) sprite->dst.y--;
		if (sglf::Input::down[0]) sprite->dst.y++;
		if (sglf::Input::left[0]) sprite->dst.x--;
		if (sglf::Input::right[0]) sprite->dst.x++;

		// Render to target
		sglf::Graphics::setRenderTexture(renderTexture);
		sglf::Graphics::clearScreen({255, 255, 255, 255});

		sprite->batch();
		sprite->texture->draw();

		// Render to default "canvas"
		sglf::Graphics::setRenderTexture();
		sglf::Graphics::clearScreen({255, 143, 119, 255});
		renderTexture->batch();
		renderTexture->texture->draw();

		auto oldPos = sprite->dst;
		sprite->dst.x += 256;
		sprite->dst.y += 256;
		sprite->dst.z /= 2;
		sprite->dst.w /= 2;
		sprite->batch();

		sprite->dst = oldPos;
		sprite->dst.x += 256;
		sprite->dst.y -= 128;
		sprite->dst.z *= 2;
		sprite->dst.w *= 2;
		sprite->batch();

		sprite->texture->draw();

		sprite->dst = oldPos;
		
		sglf::endFrame();
	}

	delete sound;
	delete sprite;
	delete textureAtlas;
	delete renderTexture;

	sglf::finalize();
	return 0;
}