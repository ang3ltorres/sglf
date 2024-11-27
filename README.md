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

int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  unsigned int width = 800;
  unsigned int height = 600;

  sglf::Window::initialize(width, height, "OpenGL", hInstance, nShowCmd);
  sglf::Graphics::initialize();
  sglf::Sound::initialize();

  sglf::Texture *textureAtlas = new sglf::Texture("C:/example/someTexture.png");
  sglf::Sprite *sprite = new sglf::Sprite(textureAtlas, {0, 0, 32, 32}, {400, 300, 32 * 4, 32 * 4});

  while (!sglf::Window::shouldClose())
  {
    // Update logic
    if (sglf::Input::up[0]) sprite->dst.y--;
    if (sglf::Input::down[0]) sprite->dst.y++;
    if (sglf::Input::left[0]) sprite->dst.x--;
    if (sglf::Input::right[0]) sprite->dst.x++;

    // Render to default "canvas"
    sglf::Graphics::setRenderTexture();
    sglf::Graphics::clearScreen({255, 143, 119, 255});
    sprite->batch();
    sprite->texture->draw();
    sglf::Graphics::draw();
  }

  delete sprite;
  delete textureAtlas;

  sglf::Sound::finalize();
  sglf::Graphics::finalize();
  sglf::Window::finalize();
  
  return 0;
}
```
<p align="center" style="font-size: 48px;">^__^</p>