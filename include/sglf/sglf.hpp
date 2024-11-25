#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

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

	glm::vec4 getVec4();
};

class Shader;
class RenderTexture;
class Camera;

class SGLF
{
public:
	SGLF() = delete;
	SGLF(const SGLF&) = delete;
	~SGLF() = default;
	
	static void initialize(unsigned int width, unsigned int height, const char *windowName);
	static void finalize();
	
	static void begin();

	// Window
	static GLFWwindow *window;
	static unsigned int width;
	static unsigned int height;
	
	//
	static Shader *currentShader;
	static GLuint currentVAO;
	static GLuint currentTexture;
	static Camera *currentCamera;
	static Camera *defaultCamera;

	// Time Travel stuff
	static float fps;
	static float delta;
	static bool forceClose;

	// Library Methods
	static bool shouldClose();
	static void clearScreen(const Color &color);
	static void draw();
	static void setRenderTexture(RenderTexture *renderTexture = nullptr);
	static void setVAO(GLuint VAO);
	static void setTexture(GLuint texture);

	// Callback setting
	static void setResizeCallback(GLFWframebuffersizefun callback);

	// Internal Methods
	static void setViewport(int width, int height);
};

class Input
{
public:
	Input() = delete;
	Input(const Input&) = delete;
	~Input() = default;

	static void reset();

	static bool escape_DOWN;
	static bool up_DOWN;
	static bool left_DOWN;
	static bool down_DOWN;
	static bool right_DOWN;

	static bool escape_PRESSED;
	static bool up_PRESSED;
	static bool left_PRESSED;
	static bool down_PRESSED;
	static bool right_PRESSED;

	static bool escape_RELEASED;
	static bool up_RELEASED;
	static bool left_RELEASED;
	static bool down_RELEASED;
	static bool right_RELEASED;
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
	unsigned int width;
	unsigned int height;
	unsigned char *pixelData;

	static void initialize();
	static void finalize();

	static void getPixelData(const char *fileName, unsigned char *&buffer, unsigned int &width, unsigned int &height);
	static void freePixelData(unsigned char *&buffer);

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
