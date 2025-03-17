#include <camera.hpp>
using namespace sglf;
using namespace glm;

Camera::Camera(unsigned int width, unsigned int height)
: position(0.0f, 0.0f), zoom(1.0f), width(width), height(height), view(0.0f), projection(0.0f), viewProjection(0.0f)
{
	updateViewProjectionMatrix();
}

void Camera::setPosition(const vec2& position)
{
	this->position = position;
	updateViewProjectionMatrix();
}

void Camera::move(const vec2 &delta)
{
	position -= delta;
	updateViewProjectionMatrix();
}

void Camera::setZoom(float zoom)
{
	this->zoom = zoom;
	updateViewProjectionMatrix();
}

void Camera::updateViewProjectionMatrix()
{
	updateView();
	updateProjection();
	viewProjection = projection * view;;
}

mat4 &Camera::getViewProjectionMatrix()
{
	return viewProjection;
}

void Camera::updateView()
{
	view = translate(mat4(1.0f), vec3(position, 0.0f));
}

void Camera::updateProjection()
{
	float left   = 0.0f;
	float right  = width / zoom;
	float bottom = 0.0f;
	float top    = height / zoom;

	projection = ortho(left, right, top, bottom, -1.0f, 1.0f);
}
