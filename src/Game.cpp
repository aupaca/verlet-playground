#include "Game.h"
#include "Renderer.h"
#include "Ball.h"
#include "vec.h"

Game::Game(const GameParam& param)
    : numberGen(std::random_device()())
    , boxWidth(3, 7)
    , boxHeight(3, 7)
{
    config = param;
    renderer = nullptr;
    
    for (int i = 0; i < MULTITOUCH_LIMIT; i++)
    {
        touches[i].canceled = true;
    }
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
    renderer->setUniform("u_primaryColor", config.primaryColor);
    renderer->setUniform("u_secondaryColor", config.secondaryColor);
    solver.init(config.maxBallCount, config.ballRadius);
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
        interact(t);
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
    if ((b = solver.findObject(finger->pos)))
    {
        touch->interactionMode = MOVE_OBJECT;
        touch->clamp.target = b;
    }
    else if (finger->pressedTime() >= 0.3f)
    {
        touch->interactionMode = DROP_OBJECT;
        touch->box.width = boxWidth(numberGen);
        touch->box.height = boxHeight(numberGen);
    }
    else if (finger->moved(16.f, finger->firstPos))
    {
        touch->interactionMode = BUILD_BRIDGE;
        touch->bridge.currHead = nullptr;
        touch->bridge.finalize = false;
    }
}

void Game::interact(Touch* touch)
{
    if (touch->canceled)
    {
        return;
    }
    if (touch->interactionMode == NONE)
    {
        detectMode(touch);
    }
    
    Ball* b;
    Finger* finger = &touch->finger;
    switch (touch->interactionMode)
    {
        case MOVE_OBJECT:
            touch->clamp.target->pos = finger->pos;
            break;
            
        case DROP_OBJECT:
            if (!hasCapacity(touch->box.width * touch->box.height))
            {
                touch->canceled = true;
                return;
            }
            
            solver.addBox(finger->pos, config.ballRadius, touch->box.width, touch->box.height, PRIMARY_COLOR);
            touch->canceled = true;
            break;
        
        case BUILD_BRIDGE:
            if (!hasCapacity(1))
            {
                touch->canceled = true;
                return;
            }
            
            if (touch->bridge.currHead == nullptr)//finger->action == anut::MotionEvent::ACTION_DOWN)
            {
                touch->bridge.currHead = solver.addBall(finger->pos, config.ballRadius, PRIMARY_COLOR, true);
            }
            else if (finger->action == anut::MotionEvent::ACTION_UP)
            {
                b = solver.addBall(finger->pos, config.ballRadius, PRIMARY_COLOR, true);
                solver.link(touch->bridge.currHead, b);
                touch->canceled = true;
            }
            else if (finger->moved(2.f * config.ballRadius, touch->bridge.currHead->pos))
            {
                b = solver.addBall(finger->pos, config.ballRadius, SECONDARY_COLOR);
                solver.link(touch->bridge.currHead, b);
                touch->bridge.currHead = b;
                touch->pauseBall(b);
            }
            break;
    }
}

bool Game::hasCapacity(int ballsToAdd)
{
    if (solver.ballCount() + ballsToAdd > config.maxBallCount)
    {
        return false;
    }
    return true;
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
            touch->canceled = false;
            break;
        
        case anut::MotionEvent::ACTION_MOVE:
            finger->onMove(motion);
            break;
        
        case anut::MotionEvent::ACTION_UP:
            finger->onUp(motion);
            if (touch->interactionMode == BUILD_BRIDGE)
            {
                touch->unpauseBalls();
            }
            else
            {
                touch->canceled = true;
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
