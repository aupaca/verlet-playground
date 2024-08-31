#ifndef RENDERER_H
#define RENDERER_H
#include "vec.h"
#include "shader.h"
#include <ANUT/ANUT_graphics.h>
#include <glm/glm.hpp>
#include <vector>

class Ball;
//class BallBuffer1;
//class BallBuffer2;

class Renderer
{
public:
	Renderer(int maxBallCount);
	
	bool init();
	void loadScene(const std::vector<Ball>& objects);
	void draw();
	void setUniform(const char* name, const glm::mat4& value);
	void setUniform(const char* name, const glm::vec4& value);
	void setUniform(const char* name, int value);
	
private:
	void configTextures();
	void configBuffers();
	void loadTempBuffers(const std::vector<Ball>& objects);
	
	int ballCount;
	std::vector<BallPosition> tempPositions;
	std::vector<BallAttributes> tempProperties;
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
