#ifndef BALL_H
#define BALL_H
#include "vec.h"
#include <glm/glm.hpp>

class Ball;

void setDistance(Ball* b1, Ball* b2, float targetDist);

struct Ball 
{
    Ball(const vec& initPos, float r, const glm::vec4& fillColor, bool fixed = false);
    void applyForce(const vec& f, float dt);
    
    vec pos;
    vec oldPos;
    glm::vec4 color;
    float radius;
    bool stopped;
};


#endif
