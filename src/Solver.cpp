#include "Solver.h"
#include "Ball.h"
#include <ANUT/ANUT.h>

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
    for (int i = 0; i < 4; i++)
    {
        applyGravity(dt/4.f);
        applyCollision();
        applyLink();
    }
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
    else if (ballLeft < 0)
    {
        b->pos.x = 0.f + b->radius;
    }
}

void Solver::addBall(const vec& pos, float radius, const glm::vec4& color, bool fix)
{
    objects.emplace_back(pos, radius, color, fix);
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

int Solver::ballCount() const
{
    return objects.size();
}

Ball* Solver::getLastBall()
{
    return &objects.back();
}

