#include <text.hpp>
using namespace sglf;
using namespace glm;

Text::Text(const char *text, Font *font, glm::vec2 pos, Color color)
: Drawable::Drawable(new Texture(text, font, 1)), font(font)
{
	this->text = new char[strlen(text) + 1];
	strcpy(this->text, text);

	src         = {0, 0, texture->width, texture->height};
	dst         = {pos.x, pos.y, texture->width, texture->height};
	this->color = color;
	rotation    = 0.0f;
	updateModel();
}

Text::~Text()
{
	delete text;
	delete texture;
}

void Text::setText(const char *text)
{
	delete this->text;
	this->text = new char[strlen(text) + 1];
	strcpy(this->text, text);
	render();
}

void Text::render()
{
	delete texture;
	texture = new Texture(text, font, 1);

	src   = {0, 0, texture->width, texture->height};
	dst.z = texture->width;
	dst.w = texture->height;

	updateModel();
}
