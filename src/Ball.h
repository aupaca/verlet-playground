#ifndef BALL_H
#define BALL_H
#include "vec.h"
#include "shader.h"
#include <glm/glm.hpp>

class Ball;

void setDistance(Ball* b1, Ball* b2, float targetDist);

struct Ball 
{
    Ball(const vec& initPos, float r, const Color& fillColor, bool fixed = false);
    void applyForce(const vec& f, float dt);
    //void setVelocity(float v);
    //float getVelocity();
    
    vec pos;
    vec oldPos;
    Color color;
    float radius;
    bool stopped;
};


#endif
