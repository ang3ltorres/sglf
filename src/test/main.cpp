#include <sglf/sglf.hpp>
#include <cstdio>

static void resized(unsigned int width, unsigned int height)
{
	sglf::Graphics::defaultCamera->width  = width;
	sglf::Graphics::defaultCamera->height = height;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	unsigned int width = 800;
	unsigned int height = 600;
	
	sglf::initialize(width, height, "OpenGL", hInstance);
	sglf::Window::resizedCallback = &resized;
	
	sglf::RenderTexture *renderTexture = new sglf::RenderTexture{256, 240};
	sglf::Texture       *textureAtlas  = new sglf::Texture{"D:/sglf_res/png_test.png"};
	sglf::Sprite        *sprite        = new sglf::Sprite{textureAtlas, {0, 0, 400, 300}, {0, 0, 400, 300}};
	sglf::Sound         *sound         = new sglf::Sound{"D:/sglf_res/coin.ogg"};
	sglf::Font          *font          = new sglf::Font{"D:/sglf_res/Minecraft.ttf", "Minecraft"};
	sglf::Text          *text          = new sglf::Text{"UwU", font};

	while (!sglf::Window::shouldClose())
	{
		// Update logic
		if (sglf::Input::down[1]) sound->play();

		if (sglf::Input::up[0])    { sprite->dst.y--; sprite->updateModel(); }
		if (sglf::Input::down[0])  { sprite->dst.y++; sprite->updateModel(); }
		if (sglf::Input::left[0])  { sprite->dst.x--; sprite->updateModel(); }
		if (sglf::Input::right[0]) { sprite->dst.x++; sprite->updateModel(); }

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

		text->render();
		
		sglf::endFrame();
	}

	delete text;
	delete font;
	delete sound;
	delete sprite;
	delete textureAtlas;
	delete renderTexture;

	sglf::finalize();
	return 0;
}