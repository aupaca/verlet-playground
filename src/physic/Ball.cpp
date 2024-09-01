#include "Ball.h"
#include <cmath>

void setDistance(Ball* b1, Ball* b2, float targetDist)
{
    vec axis = b1->pos - b2->pos;
    float dist = hypotf(axis.x, axis.y);
    float frac = (dist - targetDist) / dist;
    vec displacement = frac * axis;
    if (b1->stopped)
    {
        b2->pos += displacement;
    }
    else if (b2->stopped)
    {
        b1->pos -= displacement;
    }
    else
    {
        vec halfDisplacement = 0.5f * displacement;
        b1->pos -= halfDisplacement;
        b2->pos += halfDisplacement;
    }
}

Ball::Ball(const vec& initPos, float r, const Color& fillColor, bool fixed)
{
    pos = oldPos = initPos;
    radius = r;
    color = fillColor;
    stopped = fixed;
}

void Ball::applyForce(const vec& f, float dt)
{
    if (stopped)
    {
        return;
    }
    vec vel = pos - oldPos;
    oldPos = pos;
    vec acc = f;
    pos += vel + dt * dt * acc;
}

/*void Ball::setVelocity(float v)
{
    vec axis = pos - oldPos;
    float currVel = hypotf(axis.x, axis.y);
    oldPos += (currVel - v) / currVel * axis;
}

float Ball::getVelocity()
{
    vec delta = pos - oldPos;
    return hypotf(delta.x, delta.y);
}*/
