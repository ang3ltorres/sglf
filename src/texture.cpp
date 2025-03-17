#include <texture.hpp>
using namespace sglf;
using namespace glm;

#include "graphics.hpp"
#include "util.hpp"

// *Generic Vertex data for drawing 2d stuff
struct Vertex
{
	float position[2];
	float uv[2];
};

static const Vertex vertices[] =
{
	{ { 0.0f, 0.0f }, { 0.0f, 0.0f } },
	{ { 1.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { 0.0f, 1.0f }, { 0.0f, 1.0f } },
	{ { 1.0f, 1.0f }, { 1.0f, 1.0f } }
};

static const unsigned int indices[] =
{
	0, 1, 2,
	1, 2, 3,
};

IWICImagingFactory *Texture::wicFactory;
Shader *Texture::shader;
GLuint Texture::VAO;
GLuint Texture::VBO;
GLuint Texture::EBO;
GLuint Texture::UBO_Shared;
Texture::GPU_UBO Texture::UBO_Data;

void Texture::initialize()
{
	//* VAO/Shader For 2D Texture drawing
	Texture::shader = new Shader("../shader/texture.vs", "../shader/texture.fs");

	glCreateVertexArrays(1, &Texture::VAO);

	glCreateBuffers(1, &Texture::VBO);
	glNamedBufferStorage(Texture::VBO, sizeof(vertices), vertices, 0);

	glCreateBuffers(1, &Texture::EBO);
	glNamedBufferStorage(Texture::EBO, sizeof(indices), indices, 0);

	glVertexArrayElementBuffer(Texture::VAO, Texture::EBO);

	GLuint vbufIndex = 0;
	glVertexArrayVertexBuffer(Texture::VAO, vbufIndex, Texture::VBO, 0, sizeof(Vertex));

	//! layout(location = 0) in vec2 aPos;
	GLuint aPos_location = 0;
	glVertexArrayAttribFormat(Texture::VAO, aPos_location, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(Texture::VAO, aPos_location, vbufIndex);
	glEnableVertexArrayAttrib(Texture::VAO, aPos_location);

	//! layout(location = 1) in vec2 aTexCoord;
	GLuint aTexCoord_location = 1;
	glVertexArrayAttribFormat(Texture::VAO, aTexCoord_location, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
	glVertexArrayAttribBinding(Texture::VAO, aTexCoord_location, vbufIndex);
	glEnableVertexArrayAttrib(Texture::VAO, aTexCoord_location);

	//! Fill camera buffer (UBO_Shared)
	Texture::UBO_Data.gpu_ViewProjection = Graphics::currentCamera->viewProjection;
	glCreateBuffers(1, &Texture::UBO_Shared);
	glNamedBufferData(Texture::UBO_Shared, sizeof(Texture::GPU_UBO), &Texture::UBO_Data, GL_STREAM_DRAW);

	CoCreateInstance
	(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&Texture::wicFactory
	);
}

void Texture::finalize()
{
	Texture::wicFactory->Release();
	glDeleteBuffers(1, &Texture::UBO_Shared);
	glDeleteBuffers(1, &Texture::EBO);
	glDeleteBuffers(1, &Texture::VBO);
	glDeleteVertexArrays(1, &Texture::VAO);
	delete Texture::shader;
}

void Texture::getPixelDataPNG(const char *fileName, unsigned char *&buffer, unsigned int *width, unsigned int *height)
{
	IWICBitmapDecoder *wicDecoder;
	wicFactory->CreateDecoder(GUID_ContainerFormatPng, nullptr, &wicDecoder);
	wchar_t *w_fileName = toWideString(fileName);

	IWICStream *wicStream = nullptr;
	Texture::wicFactory->CreateStream(&wicStream);
	wicStream->InitializeFromFilename((const wchar_t*)w_fileName, GENERIC_READ);

	wicDecoder->Initialize(
		wicStream,
		WICDecodeMetadataCacheOnLoad
	);

	IWICBitmapFrameDecode *wicFrame = nullptr;
	wicDecoder->GetFrame(0, &wicFrame);

	IWICFormatConverter *wicConverter = nullptr;
	Texture::wicFactory->CreateFormatConverter(&wicConverter);

	wicConverter->Initialize
	(
		wicFrame,
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);

	wicConverter->GetSize(width, height);

	unsigned int bufferSize = (*width) * (*height) * 4;
	buffer = new unsigned char[bufferSize];

	wicConverter->CopyPixels(
		nullptr,
		*width * 4,
		bufferSize,
		buffer
	);

	delete[] w_fileName;
	wicConverter->Release();
	wicFrame->Release();
	wicDecoder->Release();
}

void Texture::getPixelDataSVGFixed(const char *fileName, unsigned char *&buffer, unsigned int widthDesired, unsigned int heightDesired)
{
	plutosvg_document_t *document = plutosvg_document_load_from_file(fileName, -1, -1);
	
	// Get data in ARGB format
	plutovg_surface_t *surface = plutosvg_document_render_to_surface(document, nullptr, widthDesired, heightDesired, nullptr, nullptr, nullptr);

	unsigned int bufferSize = widthDesired * heightDesired * 4;
	unsigned char *data = plutovg_surface_get_data(surface);
	buffer = new unsigned char[bufferSize];
	std::memcpy(buffer, data, bufferSize);

	// Transform to RGBA format
	unsigned char temp;
	for (unsigned int i = 0; i < bufferSize; i += 4)
	{
		// Swap bytes directly
		temp = buffer[i];
		buffer[i] = buffer[i + 2];
		buffer[i + 2] = temp;
	}
	
	plutovg_surface_destroy(surface);
	plutosvg_document_destroy(document);
}

void Texture::getPixelDataSVGPercent(const char *fileName, unsigned char *&buffer, float percent, unsigned int *width, unsigned int *height)
{
	plutosvg_document_t *document = plutosvg_document_load_from_file(fileName, -1, -1);

	static plutovg_rect_t extents;
	plutosvg_document_extents(document, nullptr, &extents);

	*width  = extents.w * percent;
	*height = extents.h * percent;

	// Get data in ARGB format
	plutovg_surface_t *surface = plutosvg_document_render_to_surface(document, nullptr, *width, *height, nullptr, nullptr, nullptr);

	unsigned int bufferSize = (*width) * (*height) * 4;
	unsigned char *data = plutovg_surface_get_data(surface);
	buffer = new unsigned char[bufferSize];
	std::memcpy(buffer, data, bufferSize);

	// Transform to RGBA format
	unsigned char temp;
	for (unsigned int i = 0; i < bufferSize; i += 4)
	{
		// Swap bytes directly
		temp = buffer[i];
		buffer[i] = buffer[i + 2];
		buffer[i + 2] = temp;
	}
	
	plutovg_surface_destroy(surface);
	plutosvg_document_destroy(document);
}

void Texture::getPixelDataFont(const char *text, Font *font, unsigned char *&buffer, unsigned int &width, unsigned int &height)
{
	// *Convert input strings to wide strings
	wchar_t *w_text = toWideString(text);

	// *Calculate the size required for the text
	Gdiplus::Bitmap tempBitmap(1, 1);
	Gdiplus::Graphics tempGraphics(&tempBitmap);
	Gdiplus::RectF bounds;
	tempGraphics.MeasureString(w_text, -1, font->gdiFont, Gdiplus::PointF(0.0f, 0.0f), &bounds);

	// *Get needed size
	width  = (int)std::ceil(bounds.Width);
	height = (int)std::ceil(bounds.Height);

	// *Create bitmap for rendering the text
	Gdiplus::Bitmap bitmap(width, height, PixelFormat32bppARGB);
	Gdiplus::Graphics graphics(&bitmap);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
	graphics.Clear(Gdiplus::Color(255, 0, 0, 0));
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintSingleBitPerPixelGridFit);
	graphics.DrawString(w_text, -1, font->gdiFont, Gdiplus::PointF(0.0f, 0.0f), &brush);

	// *Retrieve raw pixel data
	Gdiplus::BitmapData bitmapData;
	Gdiplus::Rect rect(0, 0, width, height);
	bitmap.LockBits(&rect, Gdiplus::ImageLockMode::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);

	unsigned int *pixelsARGB = (unsigned int*)bitmapData.Scan0;
	unsigned int totalPixels = width * height;

	// *Allocate memory
	buffer = new unsigned char[totalPixels];

	// *Convert ARGB to single channel
	for (unsigned int i = 0; i < totalPixels; i++)
		buffer[i] = pixelsARGB[i];

	// *Cleanup resources
	bitmap.UnlockBits(&bitmapData);
	delete[] w_text;
}

void Texture::createTextureBuffers(int textureType)
{
	// Texture
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	if (textureType == 2)
	{
		glTextureStorage2D(id, 1, GL_R8, width, height);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, pixelData);
	}
	else
	{
		glTextureStorage2D(id, 1, GL_RGBA8, width, height);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
	}

	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Buffer
	glCreateBuffers(1, &SSBO);
	glNamedBufferData(SSBO, sizeof(GPU_SSBO) * maxInstances, nullptr, GL_STREAM_DRAW);
	SSBO_Data = new GPU_SSBO[maxInstances];

	gpu_TextureType = textureType;
	glCreateBuffers(1, &UBO);
	glNamedBufferData(UBO, sizeof(int), &gpu_TextureType, GL_STREAM_DRAW);
}

Texture::Texture(const char *fileName, unsigned int maxInstances)
: maxInstances(maxInstances), currentInstance(0)
{
	Texture::getPixelDataPNG(fileName, pixelData, &width, &height);
	createTextureBuffers(0);
}

Texture::Texture(const char *fileName, unsigned int width, unsigned int height, unsigned int maxInstances)
: width(width), height(height), maxInstances(maxInstances), currentInstance(0)
{
	Texture::getPixelDataSVGFixed(fileName, pixelData, width, height);
	createTextureBuffers(0);
}

Texture::Texture(const char *fileName, float percent, unsigned int maxInstances)
: maxInstances(maxInstances), currentInstance(0)
{
	Texture::getPixelDataSVGPercent(fileName, pixelData, percent, &width, &height);
	createTextureBuffers(0);
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int maxInstances)
: width(width), height(height), maxInstances(maxInstances), currentInstance(0)
{
	// *Create empty texture
	pixelData = nullptr;
	createTextureBuffers(1);
}

Texture::Texture(const char *text, Font *font, unsigned int maxInstances)
: maxInstances(maxInstances), currentInstance(0)
{
	Texture::getPixelDataFont(text, font, pixelData, width, height);
	createTextureBuffers(2);
}

Texture::~Texture()
{
	glDeleteBuffers(1, &UBO);
	glDeleteBuffers(1, &SSBO);
	glDeleteTextures(1, &id);
	SSBO_Data = new GPU_SSBO[maxInstances];
	delete[] SSBO_Data;
	delete[] pixelData;
}

void Texture::draw()
{
	Texture::shader->use();
	Graphics::setVAO(Texture::VAO);
	Graphics::setTexture(id);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GPU_SSBO) * currentInstance, SSBO_Data);
	
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, Texture::UBO_Shared);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBO);
	
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, currentInstance);
	currentInstance = 0;
}
