# SGLF (Simple GL Framework)

Minimalist C++ library containing all you need to create simple 2D applications using mostly only the win32 API and modern OpenGL.

<p align="center">
  <img src="logo.svg" alt="logo" width="400">
</p>

## Dependencies
- [GLM](https://github.com/g-truc/glm)
- [LIBOGG](https://xiph.org/downloads/)
- [LIBVORBIS](https://xiph.org/downloads/)

## Example Code
```cpp
#include <sglf/sglf.hpp>

int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  unsigned int width = 800;
  unsigned int height = 600;
  
  sglf::initialize(width, height, "OpenGL", hInstance, nCmdShow);
  
  sglf::RenderTexture *renderTexture = new sglf::RenderTexture(256, 240);
  sglf::Texture *textureAtlas = new sglf::Texture("someTexture.png");
  sglf::Sprite *sprite = new sglf::Sprite(textureAtlas, {0, 0, 23, 22}, {0, 0, 23 * 8, 22 * 8});
  sglf::Sound *sound = new sglf::Sound("someSound.ogg");

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
    
    sglf::Graphics::draw();
    sglf::Input::reset();
  }

  delete sound;
  delete sprite;
  delete textureAtlas;
  delete renderTexture;

  sglf::finalize();
  
  return 0;
}
```
<p align="center" style="font-size: 48px;">^__^</p>