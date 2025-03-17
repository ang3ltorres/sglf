#include <drawable.hpp>
using namespace sglf;
using namespace glm;

#include "graphics.hpp"

Drawable::Drawable(Texture *texture)
: texture(texture), src({0, 0, texture->width, texture->height}), dst({0, 0, texture->width, texture->height}), color({255, 255, 255, 255}), rotation(0.0f)
{}

void Drawable::updateModel()
{
	model =
	{
		translate(mat4(1.0f), vec3(dst.x, dst.y, 0.0f))
			*
		rotate(mat4(1.0f), radians(rotation), {0.0f, 0.0f, 1.0f})
			*
		scale(mat4(1.0f), vec3(dst.z, dst.w, 1.0f))
	};
}

void Drawable::batch()
{
	texture->SSBO_Data[texture->currentInstance++] =
	{
		{(float)src.x / (float)texture->width, (float)src.y / (float)texture->height, (float)src.z / (float)texture->width, (float)src.w / (float)texture->height},
		{vec4{color.r, color.g, color.b, color.a}},
		{model},
	};
}
