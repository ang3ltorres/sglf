#include <graphics.hpp>
using namespace sglf;
using namespace glm;

#include "window.hpp"

#pragma region // WGL_EXTENSIONS

PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#pragma endregion // WGL_EXTENSIONS

#pragma region // OPENGL_EXTENSIONS

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

#pragma endregion // OPENGL_EXTENSIONS

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
	HWND dummy = CreateWindowExA(
		0, "STATIC", "DummyWindow", WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, nullptr, nullptr);

	HDC dc = GetDC(dummy);

	PIXELFORMATDESCRIPTOR desc =
	{
		.nSize           = sizeof(PIXELFORMATDESCRIPTOR),
		.nVersion        = 1,
		.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		.iPixelType      = PFD_TYPE_RGBA,
		.cColorBits      = 24,
		.cRedBits        = 0,
		.cRedShift       = 0,
		.cGreenBits      = 0,
		.cGreenShift     = 0,
		.cBlueBits       = 0,
		.cBlueShift      = 0,
		.cAlphaBits      = 0,
		.cAlphaShift     = 0,
		.cAccumBits      = 0,
		.cAccumRedBits   = 0,
		.cAccumGreenBits = 0,
		.cAccumBlueBits  = 0,
		.cAccumAlphaBits = 0,
		.cDepthBits      = 0,
		.cStencilBits    = 0,
		.cAuxBuffers     = 0,
		.iLayerType      = 0,
		.bReserved       = 0,
		.dwLayerMask     = 0,
		.dwVisibleMask   = 0,
		.dwDamageMask    = 0,
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
		.nSize           = sizeof(PIXELFORMATDESCRIPTOR),
		.nVersion        = 1,
		.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER,
		.iPixelType      = PFD_TYPE_RGBA,
		.cColorBits      = 24,
		.cRedBits        = 0,
		.cRedShift       = 0,
		.cGreenBits      = 0,
		.cGreenShift     = 0,
		.cBlueBits       = 0,
		.cBlueShift      = 0,
		.cAlphaBits      = 0,
		.cAlphaShift     = 0,
		.cAccumBits      = 0,
		.cAccumRedBits   = 0,
		.cAccumGreenBits = 0,
		.cAccumBlueBits  = 0,
		.cAccumAlphaBits = 0,
		.cDepthBits      = 0,
		.cStencilBits    = 0,
		.cAuxBuffers     = 0,
		.iLayerType      = 0,
		.bReserved       = 0,
		.dwLayerMask     = 0,
		.dwVisibleMask   = 0,
		.dwDamageMask    = 0,
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

	Graphics::clearScreen({255, 255, 255});

	Graphics::currentShader  = nullptr;
	Graphics::currentVAO     = 0;
	Graphics::currentTexture = 0;
	Graphics::defaultCamera  = new Camera(Window::width, Window::height);
	Graphics::currentCamera  = Graphics::defaultCamera;

	QueryPerformanceFrequency(&Graphics::frequency);
	QueryPerformanceCounter(&Graphics::lastFrameTime);
	Graphics::fps   = 0.0f;
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

void Graphics::setRenderTexture(RenderTexture *renderTexture)
{
	if (renderTexture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderTexture->FBO);
		glViewport(0, 0, renderTexture->texture->width, renderTexture->texture->height);
		Graphics::currentCamera = renderTexture->camera;
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window::width, Window::height);
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
