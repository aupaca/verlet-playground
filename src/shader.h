#ifndef SHADER_H
#define SHADER_H
#include <ANUT/ANUT_opengles.h>
#include <glm/glm.hpp>
#include <cstddef>

#define LAYOUT(bufferStruct, attr, glType)        \
            sizeof(bufferStruct::attr) / 4,       \
            glType,                               \
            GL_FALSE,                             \
            sizeof(bufferStruct),                 \
            offsetof(bufferStruct, attr)

using Color = int;
const Color PRIMARY_COLOR = 1;
const Color SECONDARY_COLOR = 2;

enum
{
	TEXQUAD_POS_ATTRIB = 0,
	TEXQUAD_TEXCOORD_ATTRIB,
	CENTERPOS_ATTRIB,
	COLOR_ATTRIB,
	RADIUS_ATTRIB
};

struct Vertex
{
    glm::vec2 pos;
	glm::vec2 texCoord;
};

struct BallPosition
{
    vec pos;
};

struct BallAttributes
{
    Color color;
    float radius;
};

#endif