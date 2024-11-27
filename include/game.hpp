#pragma once
#include <sglf/sglf.hpp>

struct alignas(16) S_CommonBitmask
{
	glm::mat4 u_ViewProjection;
	glm::mat4 u_Model;
};

class Bitmask
{
public:
	GLuint id;
	unsigned int width;
	unsigned int height;
	unsigned char *pixel;

	static void initialize();
	static void finalize();
	static sglf::Shader *shader;
	static GLuint VAO;
	static GLuint VBO;
	static GLuint EBO;

	void draw();
	
	GLuint SSBO;
	static S_CommonBitmask SSBO_Data;

	Bitmask(const char *fileName);
	Bitmask(const Bitmask &) = delete;
	~Bitmask();
};

class Entity
{
public:
	Entity() = default;
	virtual ~Entity() = default;
	virtual void update() = 0;
	virtual void draw() = 0;
};

class Solid
{
public:
	Solid(sglf::Texture *texture, glm::ivec4 src, glm::ivec4 dst);
	Solid(const sglf::Texture &) = delete;
	virtual ~Solid();

	bool collision(const Bitmask *bitmask);
	bool move(unsigned int dir, unsigned int amount, const Bitmask *bitmask);

	unsigned char *pixel;
	sglf::Sprite *sprite;
};

class Player
:
	public Solid,
	public Entity
{
public:
	Player(glm::vec2 spawnPoint);
	virtual ~Player() = default;

	virtual void update() override;
	virtual void draw() override;
};

class Game
{
public:
	unsigned int windowWidth;
	unsigned int windowHeight;

	Game(HINSTANCE &hInstance, int nCmdShow);
	Game(const Game&) = delete;
	~Game();

	void loop();
	void update();
	void draw();

	// !Resources
	static sglf::RenderTexture *renderTexture;
	static sglf::Texture *defaultAtlas;
	static Bitmask *bitmask;
	static Player *player;
	static sglf::Sound *sound;
};
