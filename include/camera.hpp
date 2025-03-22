#pragma once
#include <pch.hpp>

namespace sglf
{
	struct Camera
	{
		glm::vec2 position;
		float zoom;
	
		//
		unsigned int width;
		unsigned int height;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 viewProjection;
	
		Camera(unsigned int width, unsigned int height);
		Camera(const Camera &) = delete;
		~Camera() = default;
		void setPosition(const glm::vec2 &position);
		void move(const glm::vec2& delta);
		void setZoom(float zoom);
		void updateViewProjectionMatrix();
		glm::mat4& getViewProjectionMatrix();
		void updateView();
		void updateProjection();
	};
}
