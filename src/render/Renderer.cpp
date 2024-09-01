#include "Renderer.h"
#include "Ball.h"
#include <ANUT/ANUT.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(int maxBallCount)
	: texQuad{GL_ARRAY_BUFFER}, texQuadIndices{GL_ELEMENT_ARRAY_BUFFER}
	, positionsBuffer{GL_ARRAY_BUFFER}, propertiesBuffer{GL_ARRAY_BUFFER}
	, ballTexture{GL_TEXTURE_2D}
{
	ballCount = 0;
	configTextures();
	configBuffers();
	tempPositions.reserve(maxBallCount);
	positionsBuffer.alloc(maxBallCount * sizeof(BallPosition), GL_DYNAMIC_DRAW, nullptr);
	propertiesBuffer.alloc(maxBallCount * sizeof(BallAttributes), GL_DYNAMIC_DRAW, nullptr);
	glClearColor(0.f, 0.f, 0.f, 1.f);
}

bool Renderer::init()
{
	if (!glsl.compile("src/render/shader/base.vert", "src/render/shader/base.frag"))
	{
		return false;
	}
	if (!ballTexture.loadImage("res/circle.png", GL_RGBA, GL_RGBA, true))
	{
		return false;
	}
	shader = glsl.shaderId();
	shader.use();
	ballTexture.bindToUnit(GL_TEXTURE0);
	float w = anut::Engine::window->width();
	float h = anut::Engine::window->height();
	glm::mat4 proj = glm::ortho(0.f, w, h, 0.f);
	setUniform("u_proj", proj);
	setUniform("u_texture", 0);
	return true;
}

void Renderer::loadScene(const std::vector<Ball>& objects)
{
    loadTempBuffers(objects);
    positionsBuffer.copyData(tempPositions.data(), tempPositions.size() * sizeof(BallPosition), 0);
    propertiesBuffer.copyData(tempProperties.data(), tempProperties.size() * sizeof(BallAttributes), ballCount * sizeof(BallAttributes));
    ballCount = objects.size();
}

void Renderer::loadTempBuffers(const std::vector<Ball>& objects)
{
    tempPositions.clear();
    for (const Ball& b : objects)
    {
        tempPositions.push_back({b.pos});
    }
    
    if (ballCount == objects.size())
    {
        return;
    }
    
    tempProperties.clear();
    for (int i = ballCount; i < objects.size(); i++)
    {
        tempProperties.push_back({objects[i].color, objects[i].radius});
    }
}

void Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	layout.drawIndexedInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ballCount);
}

void Renderer::setUniform(const char* name, const glm::mat4& value)
{
	glUniformMatrix4fv(shader.uniform(name), 1, false, glm::value_ptr(value));
}

void Renderer::setUniform(const char* name, const glm::vec4& value)
{
    glUniform4fv(shader.uniform(name), 1, glm::value_ptr(value));
}

void Renderer::setUniform(const char* name, int value)
{
	glUniform1i(shader.uniform(name), value);
}

void Renderer::configTextures()
{
	const Vertex quad[] = 
	{
	    // pos                  // texCoord
		{glm::vec2(-1.f, -1.f), glm::vec2(0.f, 1.f)},
		{glm::vec2(+1.f, -1.f), glm::vec2(1.f, 1.f)},
		{glm::vec2(-1.f, +1.f), glm::vec2(0.f, 0.f)},
		{glm::vec2(+1.f, +1.f), glm::vec2(1.f, 0.f)}
	};
	const unsigned quadIndices[] = 
	{
		0, 1, 2,
		1, 3, 2
	};
	texQuad.alloc(sizeof(quad), GL_STATIC_DRAW, quad);
	texQuadIndices.alloc(sizeof(quadIndices), GL_STATIC_DRAW, quadIndices);
	ballTexture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	ballTexture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	ballTexture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ballTexture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::configBuffers()
{
	layout.indexBuffer(texQuadIndices);
	layout.vertexBuffer(TEXQUAD_POS_ATTRIB, texQuad, LAYOUT(Vertex, pos, GL_FLOAT));
	layout.vertexBuffer(TEXQUAD_TEXCOORD_ATTRIB, texQuad, LAYOUT(Vertex, texCoord, GL_FLOAT));
	layout.vertexBuffer(CENTERPOS_ATTRIB, positionsBuffer, LAYOUT(BallPosition, pos, GL_FLOAT));
	layout.vertexBuffer(COLOR_ATTRIB, propertiesBuffer, LAYOUT(BallAttributes, color, GL_INT));
	layout.vertexBuffer(RADIUS_ATTRIB, propertiesBuffer, LAYOUT(BallAttributes, radius, GL_FLOAT));
	layout.vertexAttribFrequency(CENTERPOS_ATTRIB, 1);
	layout.vertexAttribFrequency(RADIUS_ATTRIB, 1);
	layout.vertexAttribFrequency(COLOR_ATTRIB, 1);
}

