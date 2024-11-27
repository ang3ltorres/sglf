#pragma once

// WINAPI
#include <windef.h>
#include <winuser.h>
#include <windef.h>
#include <mmeapi.h>
#include <dsound.h>
#include <wincodec.h>

// OPENGL
#include <GL/gl.h>
#include <GL/wglext.h>
#include <GL/glcorearb.h>

// GLM
#include <glm/glm.hpp>

#pragma region // &WGL EXTENSIONNS

extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#pragma endregion // &WGL EXTENSIONNS

#pragma region // &OPENGL EXTENSIONNS

extern PFNGLCREATEBUFFERSPROC glCreateBuffers;
extern PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
extern PFNGLVERTEXARRAYATTRIBBINDINGPROC glVertexArrayAttribBinding;
extern PFNGLVERTEXARRAYVERTEXBUFFERPROC glVertexArrayVertexBuffer;
extern PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat;
extern PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLCREATETEXTURESPROC glCreateTextures;
extern PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;
extern PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
extern PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage;
extern PFNGLVERTEXARRAYELEMENTBUFFERPROC glVertexArrayElementBuffer;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLNAMEDBUFFERDATAPROC glNamedBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
extern PFNGLCREATEFRAMEBUFFERSPROC glCreateFramebuffers;
extern PFNGLNAMEDFRAMEBUFFERTEXTUREPROC glNamedFramebufferTexture;
extern PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC glNamedFramebufferDrawBuffers;
extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;

#pragma endregion // &OPENGL EXTENSIONNS

namespace sglf
{
	struct alignas(16) S_CommonTexture
	{
		glm::vec2 u_SrcOffset;
		glm::vec2 u_SrcSize;
		glm::vec4 u_TintColor;
		glm::mat4 u_ViewProjection;
		glm::mat4 u_Model;
		int       u_flipY;
	};

	struct Color
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		glm::vec4 getVec4() const;
	};

	class Window
	{
	public:
		Window() = delete;
		Window(const Window&) = delete;
		~Window() = default;

		static void initialize(int width, int height, const char *title, HINSTANCE hInstance, int nCmdShow);
		static void finalize();

		static bool shouldClose();
		static bool forceClose;

		static void (*resizedCallback)(unsigned int width, unsigned int height);
		
		static WNDCLASSEXA windowClass;
		static HWND hwnd;
		static MSG msg;
		static LONG_PTR savedStyle;
		static RECT savedRect;

		static unsigned int width;
		static unsigned int height;
	};

	class Input
	{
	public:
		static bool up[3];
		static bool down[3];
		static bool left[3];
		static bool right[3];

		static void keyDown(WPARAM wParam);
		static void keyUp(WPARAM wParam);

		static void resetInput();
	};

	class Sound
	{
	public:
		static void initialize();
		static void finalize();

		static LPDIRECTSOUND dsound;
		static LPDIRECTSOUNDBUFFER primaryBuffer;
		static WAVEFORMATEX waveFormat;
		static DSBUFFERDESC primaryBufferDesc;

		Sound(const char *fileName);
		~Sound();
		void preload();
		void play();
		void setVolume(long volume);
		void setPitch(unsigned int freq);
		
		LPDIRECTSOUNDBUFFER buffer;
		unsigned long bufferSize;
		char *bufferData;
	};

	class Shader
	{
	public:
		Shader(const char *vertexShader, const char *fragmentShader);
		~Shader();

		static char infoLog[512];

		void use();
		GLuint program;
	};

	class Camera
	{
	public:
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

	class Texture
	{
	public:
		GLuint id;
		GLuint width;
		GLuint height;
		unsigned char *pixelData;

		static void initialize();
		static void finalize();

		static void getPixelData(const char *fileName, unsigned char *&buffer, unsigned int &width, unsigned int &height);

		static IWICImagingFactory *wicFactory;
		
		// Generic Draw Texture 2D stuff
		static Shader *shader;
		static GLuint VAO;
		static GLuint VBO;
		static GLuint EBO;

		Texture(const char *fileName, unsigned int maxInstances = 16);
		Texture(unsigned int width, unsigned int height, unsigned int maxInstances = 1);
		~Texture();

		// Batch
		GLuint SSBO;
		S_CommonTexture *SSBO_Data;
		unsigned int maxInstances;
		unsigned int currentInstance;

		void draw();
	};

	class Sprite
	{
	public:

		Sprite(const Sprite&) = default;
		Sprite(Texture *texture, glm::ivec4 src, glm::ivec4 dst);
		~Sprite() = default;

		Texture *texture;
		glm::ivec4 src;
		glm::ivec4 dst;
		Color color;
		float rotation;

		void batch();
	};

	class RenderTexture
	{
	public:
		RenderTexture(unsigned int width, unsigned int height, Camera *camera = nullptr);
		~RenderTexture();

		GLuint FBO;
		Texture *texture;
		glm::ivec4 src;
		glm::ivec4 dst;
		Color color;
		float rotation;

		void batch();

		bool internalCamera;
		Camera *camera;
	};

	class Graphics
	{
	public:
		Graphics() = delete;
		Graphics(const Graphics&) = delete;
		~Graphics() = default;

		static void loadExtensionsWGL();
		static void loadExtensionsGL();
		
		static void initialize();
		static void finalize();
		
		static void begin();
		
		// GL Context
		static HGLRC rc;
		static HDC dc;

		// Default stuff
		static Shader *currentShader;
		static GLuint currentVAO;
		static GLuint currentTexture;
		static Camera *currentCamera;
		static Camera *defaultCamera;

		// Time Travel stuff
		static LARGE_INTEGER frequency;
		static LARGE_INTEGER lastFrameTime;
		static float fps;
		static float delta;

		// Library Methods
		static void clearScreen(const Color &color);
		static void draw();
		static void setRenderTexture(RenderTexture *renderTexture = nullptr);
		static void setVAO(GLuint VAO);
		static void setTexture(GLuint texture);
		static void updateTime();

		// Internal Methods
		static void setViewport(unsigned int width, unsigned int height);
	};
}
