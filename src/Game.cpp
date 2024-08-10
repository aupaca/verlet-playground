#include "Game.h"
#include "Renderer.h"
#include "Ball.h"
#include "vec.h"

Game::Game(const GameParam& param)
    : numberGen(1)
    , radiusRange(param.minBallRadius, param.maxBallRadius)
{
    config = param;
    renderer = nullptr;
}

Game::~Game()
{
    
}

bool Game::initGraphics()
{
    renderer = new Renderer(config.maxBallCount);
    if (!renderer->init())
    {
        delete renderer;
        renderer = nullptr;
        return false;
    }
    solver.init(config.maxBallCount, config.maxBallRadius);
    return true;
}

void Game::deinitGraphics()
{
    delete renderer;
}

void Game::update(float dt)
{
    for (int i = 0; i < MULTITOUCH_LIMIT; i++)
    {
        Touch* t = &touches[i];
        if (t->finger.isPressed())
        {
            dropBall(t);
        }
    }
    solver.update(dt);
}

void Game::detectMode(Touch* touch)
{
    Finger* finger = &touch->finger;
    
    if (finger->pressedTime() >= 0.3f)
    {
        touch->dropMode = DROP;
    }
    else if (finger->moved(config.ropeRadius, finger->firstPos))
    {
        touch->dropMode = BRIDGE;
        solver.addBall(finger->pos, config.ropeRadius, config.primaryColor, true);
        touch->ballTarget = solver.getLastBall();
    }
}

void Game::dropBall(Touch* touch)
{
    Finger* finger = &touch->finger;
    
    if (solver.ballCount() == config.maxBallCount)
    {
        return;
    }
    if (touch->dropMode == NONE)
    {
        detectMode(touch);
        return;
    }
    
    if (touch->dropMode == DROP)
    {
        solver.addBall(finger->pos, radiusRange(numberGen), config.primaryColor);
    }
    else if (touch->dropMode == BRIDGE && finger->moved(2 * config.ropeRadius, touch->ballTarget->pos))
    {
        solver.addBall(finger->pos, config.ropeRadius, config.secondaryColor);
        Ball* lastBall = solver.getLastBall();
        solver.link(lastBall, touch->ballTarget);
        touch->ballTarget = lastBall;
        touch->pauseBall(lastBall);
    }
}

void Game::draw()
{
    renderer->loadScene(solver.getObjects());
    renderer->draw();
}

void Game::handleTouch(const anut::MotionEvent& motion)
{
    if (motion.id >= MULTITOUCH_LIMIT)
    {
        return;
    }
    
    Touch* touch = &touches[motion.id];
    Finger* finger = &touch->finger;
    switch (motion.action)
    {
        case anut::MotionEvent::ACTION_DOWN:
            finger->onDown(motion);
            touch->dropMode = NONE;
            break;
        
        case anut::MotionEvent::ACTION_MOVE:
            finger->onMove(motion);
            break;
        
        case anut::MotionEvent::ACTION_UP:
            finger->onUp(motion);
            if (touch->dropMode == BRIDGE)
            {
                solver.addBall(finger->pos, config.ropeRadius, config.primaryColor, true);
                solver.link(solver.getLastBall(), touch->ballTarget);
                touch->unpauseBalls();
            }
            break;
    }
}

void Touch::pauseBall(Ball* b)
{
    b->stopped = true;
    b->oldPos = b->pos;
    tempStoppedBalls.push_back(b);
}

void Touch::unpauseBalls()
{
    for (Ball* b : tempStoppedBalls)
    {
        b->stopped = false;
    }
    tempStoppedBalls.clear();
}
