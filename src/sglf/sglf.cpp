#include <sglf.hpp>

// WINAPI
#include <objbase.h>

// STD
#include <cstdio>

// OGG VORBIS
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

// GLM
#include <glm/gtc/type_ptr.hpp>

// FREETYPE
#include <freetype/freetype.h>

using namespace glm;
using namespace sglf;

#pragma region // &WGL EXTENSIONNS

PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#pragma endregion // &WGL EXTENSIONNS

#pragma region // &OPENGL EXTENSIONNS

PFNGLCREATEBUFFERSPROC glCreateBuffers;
PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
PFNGLVERTEXARRAYVERTEXBUFFERPROC glVertexArrayVertexBuffer;
PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat;
PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLCREATETEXTURESPROC glCreateTextures;
PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;
PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage;
PFNGLVERTEXARRAYELEMENTBUFFERPROC glVertexArrayElementBuffer;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLNAMEDBUFFERDATAPROC glNamedBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
PFNGLCREATEFRAMEBUFFERSPROC glCreateFramebuffers;
PFNGLNAMEDFRAMEBUFFERTEXTUREPROC glNamedFramebufferTexture;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC glNamedFramebufferDrawBuffers;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
PFNGLACTIVETEXTUREPROC glActiveTexture;

#pragma endregion // &OPENGL EXTENSIONNS

#pragma region COLOR

vec4 Color::getVec4() const
{
	return vec4({r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f});
}

#pragma endregion COLOR

#pragma region WINDOW

bool Window::forceClose;
void (*Window::resizedCallback)(unsigned int width, unsigned int height);

WNDCLASSEXA Window::windowClass;
HWND Window::hwnd;
MSG Window::msg;
LONG_PTR Window::savedStyle;
RECT Window::savedRect;

unsigned int Window::width;
unsigned int Window::height;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_SIZE:
		{
			static RECT rc;

			GetClientRect(Window::hwnd, &rc);

			Window::width  = rc.right - rc.left;
			Window::height = rc.bottom - rc.top;

			if (Window::resizedCallback)
				Window::resizedCallback(Window::width, Window::height);

			return 0;
		}

		case WM_KEYDOWN:
		{
			Input::keyDown(wParam);
			return 0;
		}

		case WM_KEYUP:
		{
			Input::keyUp(wParam);
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			Input::mousePos.x = LOWORD(lParam);
			Input::mousePos.y = HIWORD(lParam);
			return 0;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::initialize(int width, int height, const char *title, HINSTANCE hInstance)
{
	Window::forceClose = false;
	Window::resizedCallback = nullptr;
	Window::width = width;
	Window::height = height;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEXA));
	windowClass.cbSize        = sizeof(WNDCLASSEXA);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance     = hInstance;
	windowClass.lpfnWndProc   = WindowProc;
	windowClass.lpszClassName = "MainWindow";
	windowClass.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	windowClass.style         = CS_HREDRAW | CS_VREDRAW;

	RegisterClassExA(&windowClass);
	RECT rect = { 0, 0, (LONG)Window::width, (LONG)Window::height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	// Calculate window position to center it on the screen
	int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = (screenWidth - (rect.right - rect.left)) / 2;
	int posY = (screenHeight - (rect.bottom - rect.top)) / 2;

	Window::hwnd = CreateWindowExA(
		WS_EX_OVERLAPPEDWINDOW, windowClass.lpszClassName, title, WS_OVERLAPPEDWINDOW,
		posX, posY, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, hInstance, nullptr
	);
	
	ShowWindow(Window::hwnd, SW_NORMAL);
	SetForegroundWindow(Window::hwnd);
	SetActiveWindow(Window::hwnd);
	SetFocus(Window::hwnd);

	savedStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
	GetWindowRect(hwnd, &savedRect);

	ZeroMemory(&Window::msg, sizeof(MSG));
	Window::msg.message = WM_NULL;
}

void Window::finalize()
{
	UnregisterClassA(windowClass.lpszClassName, windowClass.hInstance);
	DestroyWindow(Window::hwnd);
}

bool Window::shouldClose()
{
	if (Window::forceClose)
		return true;

	while (PeekMessage(&Window::msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (Window::msg.message == WM_QUIT)
			return true;

		TranslateMessage(&Window::msg);
		DispatchMessage(&Window::msg);
	}

	return false;
}

#pragma endregion WINDOW

#pragma region INPUT

bool Input::up[3];
bool Input::down[3];
bool Input::left[3];
bool Input::right[3];
uvec2 Input::mousePos;

void Input::keyDown(WPARAM wParam)
{
	switch (wParam)
	{
		case VK_ESCAPE:
			Window::forceClose = true;
			return;

		case VK_UP   : (!Input::up[0])    ? (Input::up[0] = Input::up[1] = true)      : Input::up[0];    break;
		case VK_DOWN : (!Input::down[0])  ? (Input::down[0] = Input::down[1] = true)  : Input::down[0];  break;
		case VK_LEFT : (!Input::left[0])  ? (Input::left[0] = Input::left[1] = true)  : Input::left[0];  break;
		case VK_RIGHT: (!Input::right[0]) ? (Input::right[0] = Input::right[1] = true): Input::right[0]; break;
	}
}

void Input::keyUp(WPARAM wParam)
{
	switch (wParam)
	{
		case VK_UP:    Input::up[0] = false;    Input::up[2]    = true; break;
		case VK_DOWN:  Input::down[0] = false;  Input::down[2]  = true; break;
		case VK_LEFT:  Input::left[0] = false;  Input::left[2]  = true; break;
		case VK_RIGHT: Input::right[0] = false; Input::right[2] = true; break;
	}
}

void Input::reset()
{
	Input::up[1] = false;
	Input::down[1] = false;
	Input::left[1] = false;
	Input::right[1] = false;

	Input::up[2] = false;
	Input::down[2] = false;
	Input::left[2] = false;
	Input::right[2] = false;
}

#pragma endregion INPUT

#pragma region SOUND

LPDIRECTSOUND Sound::dsound;
LPDIRECTSOUNDBUFFER Sound::primaryBuffer;
WAVEFORMATEX Sound::waveFormat;
DSBUFFERDESC Sound::primaryBufferDesc;

void Sound::initialize()
{
	DirectSoundCreate(nullptr, &Sound::dsound, nullptr);
	Sound::dsound->SetCooperativeLevel(Window::hwnd, DSSCL_PRIORITY);

	ZeroMemory(&Sound::waveFormat, sizeof(WAVEFORMATEX));
	Sound::waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	Sound::waveFormat.nChannels = 2;
	Sound::waveFormat.nSamplesPerSec = 44100;
	Sound::waveFormat.wBitsPerSample = 16;
	Sound::waveFormat.nBlockAlign = (Sound::waveFormat.nChannels * Sound::waveFormat.wBitsPerSample) / 8;
	Sound::waveFormat.nAvgBytesPerSec = Sound::waveFormat.nSamplesPerSec * Sound::waveFormat.nBlockAlign;

	ZeroMemory(&Sound::primaryBufferDesc, sizeof(DSBUFFERDESC));
	Sound::primaryBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	Sound::primaryBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	Sound::dsound->CreateSoundBuffer(&Sound::primaryBufferDesc, &Sound::primaryBuffer, nullptr);
	Sound::primaryBuffer->SetFormat(&Sound::waveFormat);
}

void Sound::finalize()
{
	Sound::primaryBuffer->Release();
	Sound::dsound->Release();
}

Sound::Sound(const char *fileName)
{
	OggVorbis_File vorbisFile;
	ov_fopen(fileName, &vorbisFile);
	bufferSize = ov_pcm_total(&vorbisFile, -1) * Sound::waveFormat.nBlockAlign;

	DSBUFFERDESC secondaryBufferDes;
	ZeroMemory(&secondaryBufferDes, sizeof(DSBUFFERDESC));
	secondaryBufferDes.dwSize = sizeof(DSBUFFERDESC);
	secondaryBufferDes.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	secondaryBufferDes.dwBufferBytes = bufferSize;
	secondaryBufferDes.lpwfxFormat = &Sound::waveFormat;

	Sound::dsound->CreateSoundBuffer(&secondaryBufferDes, &buffer, nullptr);
	buffer->SetFormat(&Sound::waveFormat);
	buffer->Lock(0, bufferSize, (LPVOID*)&bufferData, &bufferSize, nullptr, 0, 0);

	unsigned long bytesRead;
	unsigned long totalRead = 0;

	while (totalRead < bufferSize) {

		bytesRead = ov_read(&vorbisFile, bufferData + totalRead, bufferSize - totalRead, 0, 2, 1, nullptr);
		
		if (bytesRead <= 0)
			break;

		totalRead += bytesRead;
	}
	buffer->Unlock(bufferData, totalRead, nullptr, 0);
	ov_clear(&vorbisFile);
	preload();
}

Sound::~Sound()
{
	buffer->Release();
}

void Sound::preload()
{
	buffer->Play(0, 0, 0);
	buffer->SetCurrentPosition(0);
	buffer->Stop();
}

void Sound::play()
{
	buffer->SetCurrentPosition(0);
	buffer->Play(0, 0, 0);
}

void Sound::setVolume(long volume)
{
	buffer->SetVolume(volume);
}

void Sound::setPitch(unsigned int freq)
{
	buffer->SetFrequency(freq);
}

#pragma endregion SOUND

#pragma region SHADER

GLchar Shader::infoLog[512];

static char* readFile(const char *fileName)
{
	FILE *file; 
	long size;
	char *text;

	file = fopen(fileName, "rb");
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	text = new char[size + 1];
	text[size] = '\0';
	fseek(file, 0L, SEEK_SET);
	fread(text, 1, size, file);
	fclose(file);
	return text;
}

static GLuint compileShader(GLenum type, const char *source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, Shader::infoLog);
		printf("Shader compilation error:\n%s\n", Shader::infoLog);
	}
	return shader;
}

Shader::Shader(const char *vertexShader, const char *fragmentShader)
{
	char *vertexCode = readFile(vertexShader);
	char *fragmentCode = readFile(fragmentShader);

	GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
	GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, Shader::infoLog);
		printf("Shader linking error:\n%s\n", Shader::infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	delete[] vertexCode;
	delete[] fragmentCode;
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::use()
{
	if (Graphics::currentShader != this) {

		Graphics::currentShader = this;
		glUseProgram(program);
	}
}

#pragma endregion SHADER

#pragma region CAMERA

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

#pragma endregion CAMERA

#pragma region TEXTURE

struct Vertex
{
	float position[2];
	float uv[2];
};

static const Vertex vertices[] = {
	{ { 0.0f, 0.0f }, { 0.0f, 0.0f } },
	{ { 1.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { 0.0f, 1.0f }, { 0.0f, 1.0f } },
	{ { 1.0f, 1.0f }, { 1.0f, 1.0f } }
};

static const unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3,
};

IWICImagingFactory *Texture::wicFactory;
Shader *Texture::shader;
GLuint Texture::VAO;
GLuint Texture::VBO;
GLuint Texture::EBO;

static LPCWSTR toWideString(const char *string)
{
	if (!string)
		return nullptr;

	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, string, -1, nullptr, 0);
	if (bufferSize == 0)
		return nullptr;

	wchar_t *wideString = new wchar_t[bufferSize];
	MultiByteToWideChar(CP_UTF8, 0, string, -1, wideString, bufferSize);
	return wideString;
}

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
	glDeleteBuffers(1, &Texture::EBO);
	glDeleteBuffers(1, &Texture::VBO);
	glDeleteVertexArrays(1, &Texture::VAO);
	delete Texture::shader;
}

void Texture::getPixelData(const char *fileName, unsigned char *&buffer, unsigned int &width, unsigned int &height)
{
	IWICBitmapDecoder *wicDecoder;
	wicFactory->CreateDecoder(GUID_ContainerFormatPng, nullptr, &wicDecoder);
	LPCWSTR w_fileName = toWideString(fileName);

	IWICStream *wicStream = nullptr;
	Texture::wicFactory->CreateStream(&wicStream);
	wicStream->InitializeFromFilename(w_fileName, GENERIC_READ);

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

	wicConverter->GetSize(&width, &height);

	unsigned int bufferSize = width * height * 4;
	buffer = new unsigned char[bufferSize];

	wicConverter->CopyPixels(
		nullptr,
		width * 4,
		bufferSize,
		buffer
	);

	delete[] w_fileName;
	wicConverter->Release();
	wicFrame->Release();
	wicDecoder->Release();
}

Texture::Texture(const char *fileName, unsigned int maxInstances)
: maxInstances(maxInstances), currentInstance(0)
{
	Texture::getPixelData(fileName, pixelData, width, height);

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	flipY = 0;
	glCreateBuffers(1, &UBO);
	glNamedBufferData(UBO, sizeof(int), &flipY, GL_STREAM_DRAW);

	glCreateBuffers(1, &SSBO);
	glNamedBufferData(SSBO, sizeof(S_CommonTexture) * maxInstances, nullptr, GL_STREAM_DRAW);
	SSBO_Data = new S_CommonTexture[maxInstances];
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int maxInstances)
: width(width), height(height), maxInstances(maxInstances), currentInstance(0)
{
	unsigned char color[4] = { 0, 0, 0, 255 };
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, GL_RGBA8, width, height);
	glClearTexSubImage(id, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	flipY = 1;
	glCreateBuffers(1, &UBO);
	glNamedBufferData(UBO, sizeof(int), &flipY, GL_STREAM_DRAW);

	glCreateBuffers(1, &SSBO);
	glNamedBufferData(SSBO, sizeof(S_CommonTexture) * maxInstances, nullptr, GL_STREAM_DRAW);
	SSBO_Data = new S_CommonTexture[maxInstances];

	pixelData = nullptr;
}

Texture::~Texture()
{
	glDeleteBuffers(1, &UBO);
	glDeleteBuffers(1, &SSBO);
	glDeleteTextures(1, &id);
	delete[] SSBO_Data;
	delete[] pixelData;
}

void Texture::draw()
{
	Texture::shader->use();
	Graphics::setVAO(Texture::VAO);
	Graphics::setTexture(id);
	
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
	// glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &flipY);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(S_CommonTexture) * currentInstance, SSBO_Data);
	
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, currentInstance);
	currentInstance = 0;
}

#pragma endregion TEXTURE

#pragma region SPRITE

Sprite::Sprite(Texture *texture, ivec4 src, ivec4 dst)
: texture(texture), src(src), dst(dst), color({255, 255, 255, 255}), rotation(0.0f)
{
	updateModel();
}

void Sprite::updateModel()
{
	model = {translate(mat4(1.0f), vec3(dst.x, dst.y, 0.0f)) * rotate(mat4(1.0f), radians(rotation), {0.0f, 0.0f, 1.0f}) * scale(mat4(1.0f), vec3(dst.z, dst.w, 1.0f))};
}

void Sprite::setSrcRect(ivec4 src)
{
	this->src = src;
	updateModel();
}

void Sprite::setDstRect(ivec4 dst)
{
	this->dst = dst;
	updateModel();
}

void Sprite::setPosition(ivec2 position)
{
	dst.x = position.x;
	dst.y = position.y;
	updateModel();
}

void Sprite::setSize(ivec2 size)
{
	dst.z = size.x;
	dst.w = size.y;
	updateModel();
}

void Sprite::setRotation(float rotation)
{
	this->rotation = rotation;
	updateModel();
}

void Sprite::batch()
{
	texture->SSBO_Data[texture->currentInstance++] =
	{
		{(float)src.x / (float)texture->width, (float)src.y / (float)texture->height, (float)src.z / (float)texture->width, (float)src.w / (float)texture->height},
		{color.getVec4()},
		{Graphics::currentCamera->getViewProjectionMatrix()},
		{model},
	};
}

#pragma endregion SPRITE

#pragma region RENDER_TEXTURE

RenderTexture::RenderTexture(unsigned int width, unsigned int height, Camera *camera)
{
	rotation = 0.0f;
	color = {255, 255, 255, 255};
	texture = new Texture(width, height);

	// Camera
	internalCamera = camera ? false : true;
	this->camera = camera ? camera : new Camera(width, height);

	this->src = {0, 0, width, height};
	this->dst = {0, 0, width, height};

	glCreateFramebuffers(1, &FBO);
	glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texture->id, 0);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glNamedFramebufferDrawBuffers(FBO, 1, drawBuffers);
}

RenderTexture::~RenderTexture()
{
	if (internalCamera) delete camera;
	delete texture;
	glDeleteFramebuffers(1, &FBO);
}

void RenderTexture::batch()
{
	texture->SSBO_Data[texture->currentInstance++] = {
		{(float)src.x / (float)texture->width, (float)src.y / (float)texture->height, (float)src.z / (float)texture->width, (float)src.w / (float)texture->height},
		{color.getVec4()},
		{Graphics::currentCamera->getViewProjectionMatrix()},
		{translate(mat4(1.0f), vec3(dst.x, dst.y, 0.0f)) * rotate(mat4(1.0f), radians(rotation), {0.0f, 0.0f, 1.0f}) * scale(mat4(1.0f), vec3(dst.z, dst.w, 1.0f))},
	};
}

#pragma endregion RENDER_TEXTURE

#pragma region FONT

Font::Font(const char *fileName, const char *name)
{
	this->fileName = new char[strlen(fileName) + 1];
	strcpy(this->fileName, fileName);

	AddFontResourceEx(fileName, FR_PRIVATE, nullptr);
	font = CreateFontA
	(
		48,
		0,
		0,
		0,
		FW_DONTCARE,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		name
	);
}

Font::~Font()
{
	DeleteObject(font);
	RemoveFontResourceEx(fileName, FR_PRIVATE, NULL);
	delete[] fileName;
}

#pragma endregion FONT

#pragma region TEXT

Text::~Text()
{

}

#pragma endregion TEXT

#pragma region GRAPHICS

HGLRC Graphics::rc;
HDC Graphics::dc;
Shader *Graphics::currentShader;
GLuint Graphics::currentVAO;
GLuint Graphics::currentTexture;
Camera *Graphics::currentCamera;
Camera *Graphics::defaultCamera;
LARGE_INTEGER Graphics::frequency;
LARGE_INTEGER Graphics::lastFrameTime;
float Graphics::fps;
float Graphics::delta;

void Graphics::loadExtensionsWGL()
{
	HWND dummy = CreateWindowExW(
		0, L"STATIC", L"DummyWindow", WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, nullptr, nullptr);

	HDC dc = GetDC(dummy);

	PIXELFORMATDESCRIPTOR desc =
	{
		.nSize = sizeof(PIXELFORMATDESCRIPTOR),
		.nVersion = 1,
		.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		.iPixelType = PFD_TYPE_RGBA,
		.cColorBits = 24,
		.cRedBits = 0,
		.cRedShift = 0,
		.cGreenBits = 0,
		.cGreenShift = 0,
		.cBlueBits = 0,
		.cBlueShift = 0,
		.cAlphaBits = 0,
		.cAlphaShift = 0,
		.cAccumBits = 0,
		.cAccumRedBits = 0,
		.cAccumGreenBits = 0,
		.cAccumBlueBits = 0,
		.cAccumAlphaBits = 0,
		.cDepthBits = 0,
		.cStencilBits = 0,
		.cAuxBuffers = 0,
		.iLayerType = 0,
		.bReserved = 0,
		.dwLayerMask = 0,
		.dwVisibleMask = 0,
		.dwDamageMask = 0,
	};

	int format = ChoosePixelFormat(dc, &desc);
	DescribePixelFormat(dc, format, sizeof(PIXELFORMATDESCRIPTOR), &desc);

	SetPixelFormat(dc, format, &desc);

	HGLRC rc = wglCreateContext(dc);

	wglMakeCurrent(dc, rc);
	wglGetExtensionsStringARB  = (PFNWGLGETEXTENSIONSSTRINGARBPROC)  (void*)wglGetProcAddress("wglGetExtensionsStringARB");
	wglChoosePixelFormatARB    = (PFNWGLCHOOSEPIXELFORMATARBPROC)    (void*)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) (void*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglSwapIntervalEXT         = (PFNWGLSWAPINTERVALEXTPROC)         (void*)wglGetProcAddress("wglSwapIntervalEXT");

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(rc);
	ReleaseDC(dummy, dc);
	DestroyWindow(dummy);
}

void Graphics::loadExtensionsGL()
{
	glCreateBuffers               = (PFNGLCREATEBUFFERSPROC)               (void*)wglGetProcAddress("glCreateBuffers");
	glNamedBufferStorage          = (PFNGLNAMEDBUFFERSTORAGEPROC)          (void*)wglGetProcAddress("glNamedBufferStorage");
	glBindVertexArray             = (PFNGLBINDVERTEXARRAYPROC)             (void*)wglGetProcAddress("glBindVertexArray");
	glCreateVertexArrays          = (PFNGLCREATEVERTEXARRAYSPROC)          (void*)wglGetProcAddress("glCreateVertexArrays");
	glVertexArrayAttribBinding    = (PFNGLVERTEXARRAYATTRIBBINDINGPROC)    (void*)wglGetProcAddress("glVertexArrayAttribBinding");
	glVertexArrayVertexBuffer     = (PFNGLVERTEXARRAYVERTEXBUFFERPROC)     (void*)wglGetProcAddress("glVertexArrayVertexBuffer");
	glVertexArrayAttribFormat     = (PFNGLVERTEXARRAYATTRIBFORMATPROC)     (void*)wglGetProcAddress("glVertexArrayAttribFormat");
	glEnableVertexArrayAttrib     = (PFNGLENABLEVERTEXARRAYATTRIBPROC)     (void*)wglGetProcAddress("glEnableVertexArrayAttrib");
	glGetProgramiv                = (PFNGLGETPROGRAMIVPROC)                (void*)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog           = (PFNGLGETPROGRAMINFOLOGPROC)           (void*)wglGetProcAddress("glGetProgramInfoLog");
	glCreateTextures              = (PFNGLCREATETEXTURESPROC)              (void*)wglGetProcAddress("glCreateTextures");
	glTextureParameteri           = (PFNGLTEXTUREPARAMETERIPROC)           (void*)wglGetProcAddress("glTextureParameteri");
	glTextureStorage2D            = (PFNGLTEXTURESTORAGE2DPROC)            (void*)wglGetProcAddress("glTextureStorage2D");
	glTextureSubImage2D           = (PFNGLTEXTURESUBIMAGE2DPROC)           (void*)wglGetProcAddress("glTextureSubImage2D");
	glDeleteFramebuffers          = (PFNGLDELETEFRAMEBUFFERSPROC)          (void*)wglGetProcAddress("glDeleteFramebuffers");
	glBindFramebuffer             = (PFNGLBINDFRAMEBUFFERPROC)             (void*)wglGetProcAddress("glBindFramebuffer");
	glCreateShader                = (PFNGLCREATESHADERPROC)                (void*)wglGetProcAddress("glCreateShader");
	glShaderSource                = (PFNGLSHADERSOURCEPROC)                (void*)wglGetProcAddress("glShaderSource");
	glCompileShader               = (PFNGLCOMPILESHADERPROC)               (void*)wglGetProcAddress("glCompileShader");
	glGetShaderiv                 = (PFNGLGETSHADERIVPROC)                 (void*)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog            = (PFNGLGETSHADERINFOLOGPROC)            (void*)wglGetProcAddress("glGetShaderInfoLog");
	glCreateProgram               = (PFNGLCREATEPROGRAMPROC)               (void*)wglGetProcAddress("glCreateProgram");
	glAttachShader                = (PFNGLATTACHSHADERPROC)                (void*)wglGetProcAddress("glAttachShader");
	glLinkProgram                 = (PFNGLLINKPROGRAMPROC)                 (void*)wglGetProcAddress("glLinkProgram");
	glDeleteShader                = (PFNGLDELETESHADERPROC)                (void*)wglGetProcAddress("glDeleteShader");
	glDeleteProgram               = (PFNGLDELETEPROGRAMPROC)               (void*)wglGetProcAddress("glDeleteProgram");
	glUseProgram                  = (PFNGLUSEPROGRAMPROC)                  (void*)wglGetProcAddress("glUseProgram");
	glClearTexSubImage            = (PFNGLCLEARTEXSUBIMAGEPROC)            (void*)wglGetProcAddress("glClearTexSubImage");
	glVertexArrayElementBuffer    = (PFNGLVERTEXARRAYELEMENTBUFFERPROC)    (void*)wglGetProcAddress("glVertexArrayElementBuffer");
	glDeleteVertexArrays          = (PFNGLDELETEVERTEXARRAYSPROC)          (void*)wglGetProcAddress("glDeleteVertexArrays");
	glDeleteBuffers               = (PFNGLDELETEBUFFERSPROC)               (void*)wglGetProcAddress("glDeleteBuffers");
	glNamedBufferData             = (PFNGLNAMEDBUFFERDATAPROC)             (void*)wglGetProcAddress("glNamedBufferData");
	glBufferSubData               = (PFNGLBUFFERSUBDATAPROC)               (void*)wglGetProcAddress("glBufferSubData");
	glBindBufferBase              = (PFNGLBINDBUFFERBASEPROC)              (void*)wglGetProcAddress("glBindBufferBase");
	glCreateFramebuffers          = (PFNGLCREATEFRAMEBUFFERSPROC)          (void*)wglGetProcAddress("glCreateFramebuffers");
	glNamedFramebufferTexture     = (PFNGLNAMEDFRAMEBUFFERTEXTUREPROC)     (void*)wglGetProcAddress("glNamedFramebufferTexture");
	glNamedFramebufferDrawBuffers = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC) (void*)wglGetProcAddress("glNamedFramebufferDrawBuffers");
	glDrawElementsInstanced       = (PFNGLDRAWELEMENTSINSTANCEDPROC)       (void*)wglGetProcAddress("glDrawElementsInstanced");
	glActiveTexture               = (PFNGLACTIVETEXTUREPROC)               (void*)wglGetProcAddress("glActiveTexture");
}

void Graphics::initialize()
{
	Graphics::loadExtensionsWGL();

	int piAttribIList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB,           GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,           GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,            GL_TRUE,
		WGL_PIXEL_TYPE_ARB,               WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,               24,
		WGL_DEPTH_BITS_ARB,               24,
		WGL_STENCIL_BITS_ARB,             8,
		WGL_ACCELERATION_ARB,             WGL_FULL_ACCELERATION_ARB,
		WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
		0,
	};

	int format;
	UINT formats;
	Graphics::dc = GetDC(Window::hwnd);
	wglChoosePixelFormatARB(Graphics::dc, piAttribIList, nullptr, 1, &format, &formats);

	PIXELFORMATDESCRIPTOR desc =
	{
		.nSize = sizeof(PIXELFORMATDESCRIPTOR),
		.nVersion = 1,
		.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER,
		.iPixelType = PFD_TYPE_RGBA,
		.cColorBits = 24,
		.cRedBits = 0,
		.cRedShift = 0,
		.cGreenBits = 0,
		.cGreenShift = 0,
		.cBlueBits = 0,
		.cBlueShift = 0,
		.cAlphaBits = 0,
		.cAlphaShift = 0,
		.cAccumBits = 0,
		.cAccumRedBits = 0,
		.cAccumGreenBits = 0,
		.cAccumBlueBits = 0,
		.cAccumAlphaBits = 0,
		.cDepthBits = 0,
		.cStencilBits = 0,
		.cAuxBuffers = 0,
		.iLayerType = 0,
		.bReserved = 0,
		.dwLayerMask = 0,
		.dwVisibleMask = 0,
		.dwDamageMask = 0,
	};

	DescribePixelFormat(Graphics::dc, format, sizeof(PIXELFORMATDESCRIPTOR), &desc);
	SetPixelFormat(Graphics::dc, format, &desc);

	int attribList[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	Graphics::rc = wglCreateContextAttribsARB(Graphics::dc, nullptr, attribList);
	wglMakeCurrent(Graphics::dc, Graphics::rc);

	Graphics::loadExtensionsGL();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
	glDepthMask(GL_FALSE);
	wglSwapIntervalEXT(1);
	glActiveTexture(GL_TEXTURE0);

	Graphics::clearScreen({255, 255, 255, 255});

	Graphics::currentShader = nullptr;
	Graphics::currentVAO = 0;
	Graphics::currentTexture = 0;
	Graphics::defaultCamera = new Camera(Window::width, Window::height);
	Graphics::currentCamera = Graphics::defaultCamera;
	Graphics::setRenderTexture();

	QueryPerformanceFrequency(&Graphics::frequency);
	QueryPerformanceCounter(&Graphics::lastFrameTime);
	Graphics::fps = 0.0f;
	Graphics::delta = 0.0f;
}

void Graphics::finalize()
{
	wglMakeCurrent(GLM_NULLPTR, GLM_NULLPTR);
	wglDeleteContext(Graphics::rc);
	ReleaseDC(Window::hwnd, Graphics::dc);
}

void Graphics::clearScreen(const Color &color)
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::setViewport(unsigned int width, unsigned int height)
{
	glViewport(0, 0, width, height);
}

void Graphics::setRenderTexture(RenderTexture *renderTexture)
{
	if (renderTexture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderTexture->FBO);
		Graphics::setViewport(renderTexture->texture->width, renderTexture->texture->height);
		Graphics::currentCamera = renderTexture->camera;
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Graphics::setViewport(Window::width, Window::height);
		Graphics::currentCamera = Graphics::defaultCamera;
	}

	Graphics::currentCamera->updateViewProjectionMatrix();
}

void Graphics::setVAO(GLuint VAO)
{
	if (VAO != Graphics::currentVAO) {

		Graphics::currentVAO = VAO;
		glBindVertexArray(VAO);
	}
}

void Graphics::setTexture(GLuint texture)
{
	if (texture != Graphics::currentTexture) {

		Graphics::currentTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}

void Graphics::updateTime()
{
	static LARGE_INTEGER currentFrameTime;
	QueryPerformanceCounter(&currentFrameTime);

	Graphics::delta = (float)(currentFrameTime.QuadPart - Graphics::lastFrameTime.QuadPart) / Graphics::frequency.QuadPart;
	Graphics::lastFrameTime = currentFrameTime;

	if (Graphics::delta > 0.0f)
		Graphics::fps = 1.0f / Graphics::delta;
}

#pragma endregion GRAPHICS

void sglf::initialize(int width, int height, const char *title, HINSTANCE hInstance)
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	sglf::Window::initialize(width, height, title, hInstance);
	sglf::Graphics::initialize();
	sglf::Texture::initialize();
	sglf::Sound::initialize();
}

void sglf::finalize()
{
	sglf::Sound::finalize();
	sglf::Texture::finalize();
	sglf::Graphics::finalize();
	sglf::Window::finalize();
	CoUninitialize();
}

void sglf::endFrame()
{
	SwapBuffers(Graphics::dc);
	Input::reset();
	Graphics::updateTime();
}
