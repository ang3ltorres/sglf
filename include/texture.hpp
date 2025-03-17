#pragma once
#include <pch.hpp>

#include "shader.hpp"
#include "font.hpp"

namespace sglf
{
	struct alignas(16) S_CommonTexture
	{
		glm::vec4 u_Src;
		glm::vec4 u_TintColor;
		glm::mat4 u_ViewProjection;
		glm::mat4 u_Model;
	};
	
	class Texture
	{
	public:
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
		int type;
	
		GLuint SSBO;
		S_CommonTexture *SSBO_Data;
		unsigned int maxInstances;
		unsigned int currentInstance;
	
		void draw();
	};
}
