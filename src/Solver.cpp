#include "Solver.h"
#include "Ball.h"
#include <ANUT/ANUT.h>
#include <cmath>

Solver::Solver()
{
    
}

void Solver::init(int maxBallCount, float maxBallRadius)
{
    ballCapacity = maxBallCount;
    nextBallIndex = 0;
    objects.reserve(maxBallCount);
    screenSize.x = anut::Engine::window->width();
    screenSize.y = anut::Engine::window->height();
    collisionGrid.config(screenSize, {2 * maxBallRadius, 2 * maxBallRadius});
}

void Solver::update(float dt)
{
    applyGravity(dt);
    applyCollision();
    applyLink();
}

void Solver::applyGravity(float dt)
{
    vec gravity = {0.f, 1000.f};
    for (Ball& b : objects)
    {
        b.applyForce(gravity, dt);
        constrain(&b);
    }
}

void Solver::applyLink()
{
    for (const Link& link : links)
    {
        setDistance(link.obj1, link.obj2, link.targetDist);
    }
}

void Solver::applyCollision()
{
   collisionGrid.loadObjects(objects);
   collisionGrid.detectCollisions();
}

void Solver::constrain(Ball* b)
{
    float ballBottom = b->pos.y + b->radius;
    float ballTop = b->pos.y - b->radius;
    float ballLeft = b->pos.x - b->radius;
    float ballRight = b->pos.x + b->radius;
    
    if (ballBottom > screenSize.y)
    {
        b->pos.y = screenSize.y - b->radius;
    }
    else if (ballTop < 0.f)
    {
        b->pos.y = 0.f + b->radius;
    }
    
    if (ballRight > screenSize.x)
    {
        b->pos.x = screenSize.x - b->radius;
    }
    else if (ballLeft < 0.f)
    {
        b->pos.x = 0.f + b->radius;
    }
}

Ball* Solver::findObject(const vec& p)
{
    return collisionGrid.findObject(p);
}

Ball* Solver::addBox(const vec& centerPos, float radius, int w, int h, const Color& color)
{
    float jumpY = 2.f * radius;
    float diagonalDist = hypotf(2.f * radius, 2.f * radius);
    vec nextLinePos = centerPos - vec(0.f, (w / 2.f - 0.5f) * jumpY);
    
    Ball* firstBall = addLine(nextLinePos, w, radius, color);
    Ball* prevLine = firstBall;
    for (int i = 1; i < h; i++)
    {
        nextLinePos.y += jumpY;
        Ball* currLine = addLine(nextLinePos, w, radius, color);
        link(&currLine[0], &prevLine[0]);
        for (int i = 1; i < w; i++)
        {
            link(&currLine[i], &prevLine[i]);
            link(&currLine[i], &prevLine[i - 1], diagonalDist);
        }
        prevLine = currLine;
    }
    return firstBall;
}

Ball* Solver::addLine(const vec& centerPos, int w, float radius, const Color& color)
{
    float jumpX = 2.f * radius;
    vec initPos = centerPos - vec((w / 2.f - 0.5f) * jumpX, 0.f);
    vec nextBallPos = initPos;
    
    Ball* firstBall = addBall(nextBallPos, radius, color);
    for (int i = 1; i < w; i++)
    {
        nextBallPos.x += jumpX;
        Ball* curr = addBall(nextBallPos, radius, color);
        Ball* back = &curr[-1];
        link(curr, back);
    }
    return firstBall;
}

Ball* Solver::addBall(const vec& pos, float radius, const Color& color, bool fix)
{
    objects.emplace_back(pos, radius, color, fix);
    return &objects.back();
}

void Solver::link(Ball* b1, Ball* b2)
{
    Link conn;
    conn.obj1 = b1;
    conn.obj2 = b2;
    conn.targetDist = b1->radius + b2->radius;
    setDistance(b1, b2, conn.targetDist);
    links.push_back(conn);
}

void Solver::link(Ball* b1, Ball* b2, float minDist)
{
    Link conn;
    conn.obj1 = b1;
    conn.obj2 = b2;
    conn.targetDist = minDist;
    setDistance(b1, b2, conn.targetDist);
    links.push_back(conn);
}

int Solver::ballCount() const
{
    return objects.size();
}

Ball* Solver::getLastBall()
{
    return &objects.back();
}

