#pragma once
#include <pch.hpp>

#include "shader.hpp"
#include "font.hpp"

namespace sglf
{	
	class Texture
	{
	public:

		struct alignas(16) GPU_SSBO
		{
			glm::vec4 gpu_Src;
			glm::vec4 gpu_TintColor;
			glm::mat4 gpu_Model;
		};

		struct alignas(16) GPU_UBO
		{
			alignas(16) glm::mat4 gpu_ViewProjection;
		};

		GLuint id;
		GLuint width;
		GLuint height;
		unsigned char *pixelData;
	
		static void initialize();
		static void finalize();
	
		static void getPixelDataPNG(const char *fileName, unsigned char *&buffer, unsigned int *width, unsigned int *height);
		static void getPixelDataSVGFixed(const char *fileName, unsigned char *&buffer, unsigned int widthDesired, unsigned int heightDesired);
		static void getPixelDataSVGPercent(const char *fileName, unsigned char *&buffer, float percent, unsigned int *width, unsigned int *height);
		static void getPixelDataFont(const char *text, Font *font, unsigned char *&buffer, unsigned int &width, unsigned int &height);
	
		static IWICImagingFactory *wicFactory;
		
		// Generic Draw Texture 2D stuff
		static Shader *shader;
		static GLuint VAO;
		static GLuint VBO;
		static GLuint EBO;
		static GLuint UBO_Shared;
		static GPU_UBO UBO_Data;

		// Constructor common
		void createTextureBuffers(int textureType);
	
		Texture(const char *fileName, unsigned int maxInstances);
		Texture(const char *fileName, unsigned int width, unsigned int height, unsigned int maxInstances);
		Texture(const char *fileName, float percent, unsigned int maxInstances);
		Texture(unsigned int width, unsigned int height, unsigned int maxInstances);
		Texture(const char *text, Font *font, unsigned int maxInstances);
		~Texture();
	
		// Batch
		GLuint UBO;
		int gpu_TextureType;
	
		GLuint SSBO;
		GPU_SSBO *SSBO_Data;
		unsigned int maxInstances;
		unsigned int currentInstance;
	
		void draw();
	};
}
