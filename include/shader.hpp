#pragma once
#include <pch.hpp>

namespace sglf
{
	class Shader
	{
	public:
		Shader(const char *vertexShader, const char *fragmentShader);
		~Shader();
	
		static char infoLog[512];
	
		void use();
		GLuint program;
	};
}
