#include "game.hpp"

#include <sglf/sglf.hpp>
using namespace glm;

#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>

// *Game IMP
RenderTexture *Game::renderTexture;
Texture *Game::defaultAtlas;
Bitmask *Game::bitmask;
Player *Game::player;

static void resized(GLFWwindow *window, int width, int height)
{
	SGLF::setViewport(width, height);
	SGLF::defaultCamera->width = width;
	SGLF::defaultCamera->height = height;
	SGLF::width = width;
	SGLF::height = height;

	int scaleX = width / Game::renderTexture->texture->width;
	int scaleY = height / Game::renderTexture->texture->height;
	int scale = scaleX < scaleY ? scaleX : scaleY;
	Game::renderTexture->dst.z = scale * 256;
	Game::renderTexture->dst.w = scale * 240;

	Game::renderTexture->dst.x = (width - Game::renderTexture->dst.z) / 2;
	Game::renderTexture->dst.y = (height - Game::renderTexture->dst.w) / 2;
}

Game::Game()
{
	int width = 800;
	int height = 600;

	SGLF::initialize(width, height, "OpenGL");
	SGLF::setResizeCallback(&resized);
	Bitmask::initialize();

	Game::renderTexture = new RenderTexture(256, 240);
	Game::defaultAtlas = new Texture("C:/Users/angel/Desktop/coliisionMask.png");
	Game::bitmask = new Bitmask{"C:/Users/angel/Desktop/mask.png"};
	Game::player = new Player({0, 0});

	resized(SGLF::window, width, height);
}

Game::~Game()
{
	// &Release resources
	delete Game::player;
	delete Game::bitmask;
	delete Game::defaultAtlas;
	delete Game::renderTexture;

	// &Release main resources
	Bitmask::finalize();
	SGLF::finalize();
}

void Game::loop()
{
	while (!SGLF::shouldClose())
	{
		Game::update();
		Game::draw();
	}
}

void Game::update()
{
	if (Input::escape_PRESSED)
		SGLF::forceClose = true;

	player->update();
}

void Game::draw()
{
	//
	SGLF::setRenderTexture(renderTexture);
	SGLF::clearScreen({0, 0, 0, 255});

	player->draw();
	bitmask->draw();

	// Draw to render texture
	SGLF::setRenderTexture();
	SGLF::clearScreen({170, 170, 170, 255});
	renderTexture->batch();
	renderTexture->texture->draw();
	SGLF::draw();
}

static void getAlphaImage(const char *fileName, unsigned char *&buffer, unsigned int &width, unsigned int &height)
{
	unsigned char *pixelRGBA;

	Texture::getPixelData(fileName, pixelRGBA, width, height);

	unsigned int pixelCount = width * height;
	buffer = new unsigned char[pixelCount];

	for (unsigned int i = 0; i < pixelCount; i++)
		buffer[i] = pixelRGBA[(i * 4) + 3];

	Texture::freePixelData(pixelRGBA);
}

// *Bitmask IMP
Shader *Bitmask::shader;
GLuint Bitmask::VAO;
GLuint Bitmask::VBO;
GLuint Bitmask::EBO;
S_CommonBitmask Bitmask::SSBO_Data;

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

void Bitmask::initialize()
{
	Bitmask::shader = new Shader("../shader/bitmask.vs", "../shader/bitmask.fs");

	glCreateVertexArrays(1, &Bitmask::VAO);

	glCreateBuffers(1, &Bitmask::VBO);
	glNamedBufferStorage(Bitmask::VBO, sizeof(vertices), vertices, 0);

	glCreateBuffers(1, &Bitmask::EBO);
	glNamedBufferStorage(Bitmask::EBO, sizeof(indices), indices, 0);

	glVertexArrayElementBuffer(Bitmask::VAO, Bitmask::EBO);

	GLuint vbufIndex = 0;
	glVertexArrayVertexBuffer(Bitmask::VAO, vbufIndex, Bitmask::VBO, 0, sizeof(Vertex));

	// layout(location = 0) in vec2 aPos;
	GLuint aPos_location = 0;
	glVertexArrayAttribFormat(Bitmask::VAO, aPos_location, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(Bitmask::VAO, aPos_location, vbufIndex);
	glEnableVertexArrayAttrib(Bitmask::VAO, aPos_location);

	// layout(location = 1) in vec2 aTexCoord;
	GLuint aTexCoord_location = 1;
	glVertexArrayAttribFormat(Bitmask::VAO, aTexCoord_location, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
	glVertexArrayAttribBinding(Bitmask::VAO, aTexCoord_location, vbufIndex);
	glEnableVertexArrayAttrib(Bitmask::VAO, aTexCoord_location);
}

void Bitmask::finalize()
{
	glDeleteBuffers(1, &Bitmask::EBO);
	glDeleteBuffers(1, &Bitmask::VBO);
	glDeleteVertexArrays(1, &Bitmask::VAO);
	delete Bitmask::shader;
}

void Bitmask::draw()
{
	Bitmask::shader->use();
	SGLF::setVAO(Bitmask::VAO);
	SGLF::setTexture(id);

	Bitmask::SSBO_Data =
	{
		{SGLF::currentCamera->getViewProjectionMatrix()},
		{glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 1.0f))},
	};

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(S_CommonBitmask), (void*)&SSBO_Data);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Bitmask::Bitmask(const char *fileName)
{
	getAlphaImage(fileName, pixel, width, height);

	// Create texture
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, GL_R8, width, height);
	glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, pixel);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// SSBO
	glCreateBuffers(1, &SSBO);
	glNamedBufferData(SSBO, sizeof(S_CommonBitmask), nullptr, GL_STREAM_DRAW);
}

Bitmask::~Bitmask()
{
	delete[] pixel;
	glDeleteBuffers(1, &SSBO);
	glDeleteTextures(1, &id);
}

// *Solid IMP
Solid::Solid(Texture *texture, glm::uvec4 src, glm::uvec4 dst)
{
	sprite = new Sprite(texture, src, dst);
	pixel = new unsigned char[src.z * src.w];

	for (unsigned int y = 0; y < src.w; y++)
	for (unsigned int x = 0; x < src.z; x++)
	{
		unsigned int texIndex = ((src.y + y) * texture->width + (src.x + x)) * 4;
		pixel[y * src.z + x] = texture->pixelData[texIndex + 3];
	}
}

Solid::~Solid()
{
	delete[] pixel;
	delete sprite;
}

bool Solid::collision(const Bitmask *bitmask)
{
	static unsigned int width;
	static unsigned int height;

	width = sprite->src.z;
	height = sprite->src.w;

	for (unsigned int y = 0; y < width; y++)
	for (unsigned int x = 0; x < height; x++)
	{
		if (sprite->dst.x < 0 or sprite->dst.y < 0 or sprite->dst.x > int{bitmask->width} or sprite->dst.y > int{bitmask->height})
			continue;

		if (pixel[y * width + x] and bitmask->pixel[(y + sprite->dst.y) * bitmask->width + (x + sprite->dst.x)])
			return true;
	}

	return false;
}

bool Solid::move(unsigned int dir, unsigned int amount, const Bitmask *bitmask)
{
	static ivec2 v;

	switch (dir)
	{
		case 0:   v = {1, 0};  break;
		case 90:  v = {0, -1}; break;
		case 180: v = {-1, 0}; break;
		case 270: v = {0, 1};  break;
	}

	while (amount--)
	{
		sprite->dst.x += v.x;
		sprite->dst.y += v.y;

		if (collision(bitmask))
		{
			sprite->dst.x -= v.x;
			sprite->dst.y -= v.y;

			return true;
		}
	}

	return false;
}

Player::Player(glm::uvec2 spawnPoint)
: Solid(Game::defaultAtlas, {16, 16, 16, 16}, {spawnPoint.x, spawnPoint.y, 16, 16})
{
}

void Player::update()
{
	int vel = 1;

	if (Input::down_DOWN)  move(270, vel, Game::bitmask);
	if (Input::up_DOWN)    move(90,  vel, Game::bitmask);
	if (Input::left_DOWN)  move(180, vel, Game::bitmask);
	if (Input::right_DOWN) move(0,   vel, Game::bitmask);
}

void Player::draw()
{
	sprite->batch();
	sprite->texture->draw();
}
