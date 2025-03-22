#pragma once
#include <pch.hpp>

namespace sglf
{
	struct Shader
	{
		Shader(const char *vertexShader, const char *fragmentShader);
		~Shader();
	
		static char infoLog[512];
	
		void use();
		GLuint program;
	};
}
