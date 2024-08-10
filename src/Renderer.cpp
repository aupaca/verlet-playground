#include "Renderer.h"
#include "Ball.h"
#include <ANUT/ANUT.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(int maxBallCount)
	: texQuad{GL_ARRAY_BUFFER}, texQuadIndices{GL_ELEMENT_ARRAY_BUFFER}
	, ballPositions{GL_ARRAY_BUFFER}, ballProperties{GL_ARRAY_BUFFER}
	, ballTexture{GL_TEXTURE_2D}
{
	ballCount = 0;
	configTextures();
	configBuffers();
	ballPositions.alloc(maxBallCount * sizeof(vec), GL_DYNAMIC_DRAW, nullptr);
	ballProperties.alloc(maxBallCount * sizeof(BallProperties), GL_DYNAMIC_DRAW, nullptr);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
}

bool Renderer::init()
{
	if (!glsl.compile("src/shader/base.vert", "src/shader/base.frag"))
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
	setUniform("s_proj", proj);
	setUniform("s_texture", 0);
	return true;
}

void Renderer::loadScene(const std::vector<Ball>& objects)
{
    std::vector<vec> pos;
    std::vector<BallProperties> props;
    for (const Ball& b : objects)
    {
        pos.push_back(b.pos);
        props.push_back({b.color, b.radius});
    }
    ballCount = objects.size();
    ballPositions.copyData(pos.data(), pos.size() * sizeof(vec), 0);
    ballProperties.copyData(props.data(), props.size() * sizeof(BallProperties), 0);
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

void Renderer::setUniform(const char* name, int value)
{
	glUniform1i(shader.uniform(name), value);
}

void Renderer::configTextures()
{
	const Vertex quadVertices[] = 
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
	texQuad.alloc(sizeof(quadVertices), GL_STATIC_DRAW, quadVertices);
	texQuadIndices.alloc(sizeof(quadIndices), GL_STATIC_DRAW, quadIndices);
	ballTexture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	ballTexture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	ballTexture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ballTexture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::configBuffers()
{
	layout.indexBuffer(texQuadIndices);
	layout.vertexBuffer(POSITION_ATTRIB, texQuad, 2, GL_FLOAT, false, sizeof(Vertex), 0);
	layout.vertexBuffer(TEXCOORD_ATTRIB, texQuad, 2, GL_FLOAT, false, sizeof(Vertex), sizeof(glm::vec2));
	layout.vertexBuffer(CENTERPOS_ATTRIB, ballPositions, 2, GL_FLOAT, false, sizeof(glm::vec2), 0);
	layout.vertexBuffer(COLOR_ATTRIB, ballProperties, 4, GL_FLOAT, false, sizeof(BallProperties), 0);
	layout.vertexBuffer(RADIUS_ATTRIB, ballProperties, 1, GL_FLOAT, false, sizeof(BallProperties), sizeof(glm::vec4));
	layout.vertexAttribFrequency(CENTERPOS_ATTRIB, 1);
	layout.vertexAttribFrequency(RADIUS_ATTRIB, 1);
	layout.vertexAttribFrequency(COLOR_ATTRIB, 1);
}

