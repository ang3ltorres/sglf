#include "sglf/sglf.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstdio>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// *General DCL
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

glm::vec4 Color::getVec4()
{
	return glm::vec4({r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f});
}

GLFWwindow *SGLF::window;
unsigned int SGLF::width;
unsigned int SGLF::height;
Shader *SGLF::currentShader;
GLuint SGLF::currentVAO;
GLuint SGLF::currentTexture;
Camera *SGLF::currentCamera;
Camera *SGLF::defaultCamera;
float SGLF::fps;
float SGLF::delta;
bool SGLF::forceClose;

// *Input DCL
bool Input::escape_DOWN;
bool Input::up_DOWN;
bool Input::left_DOWN;
bool Input::down_DOWN;
bool Input::right_DOWN;

bool Input::escape_PRESSED;
bool Input::up_PRESSED;
bool Input::left_PRESSED;
bool Input::down_PRESSED;
bool Input::right_PRESSED;

bool Input::escape_RELEASED;
bool Input::up_RELEASED;
bool Input::left_RELEASED;
bool Input::down_RELEASED;
bool Input::right_RELEASED;

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE: Input::escape_DOWN = true; Input::escape_PRESSED = true; break;
			case GLFW_KEY_UP:     Input::up_DOWN     = true; Input::up_PRESSED     = true; break;
			case GLFW_KEY_LEFT:   Input::left_DOWN   = true; Input::left_PRESSED   = true; break;
			case GLFW_KEY_DOWN:   Input::down_DOWN   = true; Input::down_PRESSED   = true; break;
			case GLFW_KEY_RIGHT:  Input::right_DOWN  = true; Input::right_PRESSED  = true; break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE: Input::escape_DOWN = false; Input::escape_RELEASED = true; break;
			case GLFW_KEY_UP:     Input::up_DOWN     = false; Input::up_RELEASED     = true; break;
			case GLFW_KEY_LEFT:   Input::left_DOWN   = false; Input::left_RELEASED   = true; break;
			case GLFW_KEY_DOWN:   Input::down_DOWN   = false; Input::down_RELEASED   = true; break;
			case GLFW_KEY_RIGHT:  Input::right_DOWN  = false; Input::right_RELEASED  = true; break;
		}
	}
}

// *Shader DCL
char Shader::infoLog[512];

// *Texture DCL
Shader *Texture::shader;
GLuint Texture::VAO;
GLuint Texture::VBO;
GLuint Texture::EBO;

// *Graphics IMP
void SGLF::initialize(unsigned int width, unsigned int height, const char *windowName)
{
	SGLF::width = width;
	SGLF::height = height;
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	SGLF::window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
	glfwMakeContextCurrent(SGLF::window);
	glewInit();
	glfwSwapInterval(1);
	glfwSetKeyCallback(SGLF::window, keyCallback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);

	SGLF::fps = 0.0f;
	SGLF::delta = 0.0f;
	SGLF::currentShader = nullptr;
	SGLF::currentVAO = 0;
	SGLF::currentTexture = 0;
	SGLF::defaultCamera = new Camera(width, height);
	SGLF::currentCamera = SGLF::defaultCamera;
	SGLF::setRenderTexture();

	Texture::initialize();
}

void SGLF::finalize()
{
	Texture::finalize();
	glfwTerminate();
}

bool SGLF::shouldClose()
{
	if (SGLF::forceClose)
		glfwSetWindowShouldClose(SGLF::window, GLFW_TRUE);

	return glfwWindowShouldClose(SGLF::window);
}

void SGLF::clearScreen(const Color &color)
{
	glClearColor(float{color.r} / 255.0f, float{color.g} / 255.0f, float{color.b} / 255.0f, float{color.a} / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void SGLF::setResizeCallback(GLFWframebuffersizefun callback)
{
	glfwSetFramebufferSizeCallback(SGLF::window, callback);
}

void SGLF::setViewport(int width, int height)
{
	glViewport(0, 0, width, height);
}

void SGLF::draw()
{
	Input::reset();
	glfwSwapBuffers(SGLF::window);
	glfwPollEvents();
}

void SGLF::setRenderTexture(RenderTexture *renderTexture)
{
	if (renderTexture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderTexture->FBO);
		SGLF::setViewport(renderTexture->texture->width, renderTexture->texture->height);
		SGLF::currentCamera = renderTexture->camera;
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		SGLF::setViewport(SGLF::width, SGLF::height);
		SGLF::currentCamera = SGLF::defaultCamera;
	}

	SGLF::currentCamera->updateViewProjectionMatrix();
}

void SGLF::setVAO(GLuint VAO)
{
	if (VAO != SGLF::currentVAO) {

		SGLF::currentVAO = VAO;
		glBindVertexArray(VAO);
	}
}

void SGLF::setTexture(GLuint texture)
{
	if (texture != SGLF::currentTexture) {

		SGLF::currentTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}

// *Input IMP
void Input::reset()
{
	escape_PRESSED  = false;
	up_PRESSED      = false;
	left_PRESSED    = false;
	down_PRESSED    = false;
	right_PRESSED   = false;

	escape_RELEASED = false;
	up_RELEASED     = false;
	left_RELEASED   = false;
	down_RELEASED   = false;
	right_RELEASED  = false;
}

// *Shader IMP
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

static GLuint compileShader(unsigned int type, const char *source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (not success) {
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

	if (not success) {
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
	if (SGLF::currentShader != this) {

		SGLF::currentShader = this;
  	glUseProgram(program);
	}
}

// *Camera IMP
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

// *Texture IMP
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
}

void Texture::finalize()
{
	glDeleteBuffers(1, &Texture::EBO);
	glDeleteBuffers(1, &Texture::VBO);
	glDeleteVertexArrays(1, &Texture::VAO);
	delete Texture::shader;
}

void Texture::getPixelData(const char *fileName, unsigned char *&buffer, unsigned int &width, unsigned int &height)
{
	int x, y, comp;
	
	buffer = stbi_load(fileName, &x, &y, &comp, 4);
	width = (unsigned int)x;
	height = (unsigned int)y;
}

void Texture::freePixelData(unsigned char *&buffer)
{
	stbi_image_free(buffer);
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
	glClearTexImage(id, 0, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glCreateBuffers(1, &SSBO);
	glNamedBufferData(SSBO, sizeof(S_CommonTexture) * maxInstances, nullptr, GL_STREAM_DRAW);
	SSBO_Data = new S_CommonTexture[maxInstances];

	pixelData = nullptr;
}

Texture::~Texture()
{
	glDeleteBuffers(1, &SSBO);
	glDeleteTextures(1, &id);
	delete[] SSBO_Data;
	Texture::freePixelData(pixelData);
}

void Texture::draw()
{
	Texture::shader->use();
	SGLF::setVAO(Texture::VAO);
	SGLF::setTexture(id);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(S_CommonTexture) * currentInstance, SSBO_Data);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, currentInstance);
	
	currentInstance = 0;
}

// *Sprite IMP
Sprite::Sprite(Texture *texture, glm::ivec4 src, glm::ivec4 dst)
: texture(texture), src(src), dst(dst)
{
	rotation = 0.0f;
	color = {255, 255, 255, 255};
}

void Sprite::batch()
{
	if (texture) {
		texture->SSBO_Data[texture->currentInstance++] = {
			{float{src.x} / float{texture->width}, float{src.y} / float{texture->height}},
			{float{src.z} / float{texture->width}, float{src.w} / float{texture->height}},
			{color.getVec4()},
			{SGLF::currentCamera->getViewProjectionMatrix()},
			{translate(mat4(1.0f), vec3(dst.x, dst.y, 0.0f)) * rotate(mat4(1.0f), radians(rotation), {0.0f, 0.0f, 1.0f}) * scale(mat4(1.0f), vec3(dst.z, dst.w, 1.0f))},
			{0},
		};
	}
}

// *RenderTexture IMP
RenderTexture::RenderTexture(unsigned int width, unsigned int height, Camera *camera)
{
	rotation = 0.0f;
	color = {255, 255, 255, 255};
	texture = new Texture(width, height);

	// Camera
	internalCamera = camera ? false : true;
	this->camera = camera ? camera : new Camera(width, height);

	this->src = {0.0f, 0.0f, float{width}, float{height}};
	this->dst = {0.0f, 0.0f, float{width}, float{height}};

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
		{float{src.x} / float{texture->width}, float{src.y} / float{texture->height}},
		{float{src.z} / float{texture->width}, float{src.w} / float{texture->height}},
		{color.getVec4()},
		{SGLF::currentCamera->getViewProjectionMatrix()},
		{translate(mat4(1.0f), vec3(dst.x, dst.y, 0.0f)) * rotate(mat4(1.0f), radians(rotation), {0.0f, 0.0f, 1.0f}) * scale(mat4(1.0f), vec3(dst.z, dst.w, 1.0f))},
		{1},
	};
}
