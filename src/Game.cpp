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
            interact(t);
        }
    }
    solver.update(dt);
}

void Game::draw()
{
    renderer->loadScene(solver.getObjects());
    renderer->draw();
}

void Game::detectMode(Touch* touch)
{
    Finger* finger = &touch->finger;
    Ball* b;
    if ((b = solver.findObjectAtPoint(finger->pos)))
    {
        touch->interactionMode = MOVE_OBJECT;
        touch->ballTarget = b;
    }
    if (finger->pressedTime() >= 0.3f)
    {
        touch->interactionMode = DROP_OBJECT;
    }
    else if (finger->moved(16.f, finger->firstPos))
    {
        touch->interactionMode = BUILD_BRIDGE;
        touch->ballTarget = solver.addBall(finger->pos, config.ropeRadius, config.primaryColor, true);
    }
}

void Game::interact(Touch* touch)
{
    Finger* finger = &touch->finger;
    
    if (solver.ballCount() + 1 >= config.maxBallCount)
    {
        return;
    }
    if (touch->interactionMode == NONE)
    {
        detectMode(touch);
        return;
    }
    
    Ball* b;
    switch (touch->interactionMode)
    {
        case MOVE_OBJECT:
            touch->ballTarget->pos = finger->pos;
            break;
            
        case DROP_OBJECT:
            solver.addBox(finger->pos, config.minBallRadius, 3 + numberGen() % 5, 3 + numberGen() % 13, config.primaryColor);
            finger->isDown = false;
            break;
        
        case BUILD_BRIDGE:
            if (!finger->moved(2.f * config.ropeRadius, touch->ballTarget->pos))
            {
                return;
            }
            b = solver.addBall(finger->pos, config.ropeRadius, config.secondaryColor);
            solver.link(touch->ballTarget, b);
            touch->ballTarget = b;
            touch->pauseBall(b);
            break;
    }
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
            touch->interactionMode = NONE;
            break;
        
        case anut::MotionEvent::ACTION_MOVE:
            finger->onMove(motion);
            break;
        
        case anut::MotionEvent::ACTION_UP:
            finger->onUp(motion);
            if (touch->interactionMode == BUILD_BRIDGE)
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
