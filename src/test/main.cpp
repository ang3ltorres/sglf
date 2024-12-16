#include <sglf/sglf.hpp>

static void resized(unsigned int width, unsigned int height)
{
	sglf::Graphics::defaultCamera->width  = width;
	sglf::Graphics::defaultCamera->height = height;
}

int main()
{
	unsigned int width = 800;
	unsigned int height = 600;

	static const char *fonts[] =
	{
		"D:/sglf_res/Minecraft.ttf",
		"D:/sglf_res/Magic Retro.ttf",
		" ",
	};
	
	sglf::initialize(width, height, "OpenGL", fonts);
	sglf::Window::resizedCallback = &resized;
	
	sglf::RenderTexture *renderTexture = new sglf::RenderTexture{256, 240};
	sglf::Texture       *pngTexture    = new sglf::Texture{"D:/sglf_res/png_test.png"};
	sglf::Sprite        *pngSprite     = new sglf::Sprite{pngTexture, {0, 0, 400, 300}, {0, 0, 400, 300}};
	sglf::Texture       *svgTexture    = new sglf::Texture{"D:/sglf_res/Ghostscript_Tiger.svg", 256, 256};
	sglf::Sprite        *svgSprite     = new sglf::Sprite{svgTexture, {0, 0, 256, 256}, {0, 0, 256, 256}};
	sglf::Sound         *sound         = new sglf::Sound{"D:/sglf_res/coin.ogg"};
	sglf::Font          *font          = new sglf::Font{"Minecraft", 128/2, sglf::Font::Style::Regular, true};
	sglf::Text          *text          = new sglf::Text{"Example text", font, {0, 0}, {255, 0, 0, 255}};

	renderTexture->dst.z *= 2;
	renderTexture->dst.w *= 2;
	renderTexture->updateModel();

	while (!sglf::Window::shouldClose())
	{
		// Update logic
		if (sglf::Input::down[1]) sound->play();

		if (sglf::Input::up[0])    { pngSprite->dst.y--; pngSprite->updateModel(); }
		if (sglf::Input::down[0])  { pngSprite->dst.y++; pngSprite->updateModel(); }
		if (sglf::Input::left[0])  { pngSprite->dst.x--; pngSprite->updateModel(); }
		if (sglf::Input::right[0]) { pngSprite->dst.x++; pngSprite->updateModel(); }

		// Render to target
		sglf::Graphics::setRenderTexture(renderTexture);
		sglf::Graphics::clearScreen({255, 255, 255, 255});

		pngSprite->batch();
		pngSprite->texture->draw();

		// Render to default "canvas"
		sglf::Graphics::setRenderTexture();
		sglf::Graphics::clearScreen({255, 143, 119, 255});
		renderTexture->batch();
		renderTexture->texture->draw();

		svgSprite->batch();
		svgSprite->texture->draw();

		text->batch();
		text->texture->draw();

		sglf::endFrame();
	}

	delete svgTexture;
	delete text;
	delete font;
	delete sound;
	delete pngSprite;
	delete pngTexture;
	delete renderTexture;

	sglf::finalize();
	return 0;
}