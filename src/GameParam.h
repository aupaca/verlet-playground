#ifndef GAME_PARAM
#define GAME_PARAM
#include <glm/glm.hpp>
#include <cstdint>

struct GameParam 
{
    glm::vec4 primaryColor;
    glm::vec4 secondaryColor;
    size_t maxBallCount;
    float ropeRadius;
    float minBallRadius;
    float maxBallRadius;
};

#endif