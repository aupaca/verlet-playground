#ifndef RENDERER_H
#define RENDERER_H
#include "vec.h"
#include <ANUT/ANUT_graphics.h>
#include <glm/glm.hpp>
#include <vector>

class Ball;

struct Vertex
{
	glm::vec2 pos;
	glm::vec2 texCoord;
};

struct BallProperties
{
	glm::vec4 color;
	float radius;
};

class Renderer
{
public:
	Renderer(int maxBallCount);
	
	bool init();
	void loadScene(const std::vector<Ball>& objects);
	void draw();
	
private:
	enum
	{
		POSITION_ATTRIB = 0,
		TEXCOORD_ATTRIB,
		CENTERPOS_ATTRIB,
		COLOR_ATTRIB,
		RADIUS_ATTRIB
	};
	
	void setUniform(const char* name, const glm::mat4& value);
	void setUniform(const char* name, int value);
	void configTextures();
	void configBuffers();
	void loadTempBuffers(const std::vector<Ball>& objects);
	
	int ballCount;
	std::vector<vec> tempPositions;
	std::vector<BallProperties> tempProperties;
	anut::GLSLCompiler glsl;
	anut::Shader shader;
	anut::GPUBuffer texQuad;
	anut::GPUBuffer texQuadIndices;
	anut::GPUBuffer positionsBuffer;
	anut::GPUBuffer propertiesBuffer;
	anut::VertexLayout layout;
	anut::Texture ballTexture;
};

#endif
