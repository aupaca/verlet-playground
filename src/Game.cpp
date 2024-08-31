#include "Game.h"
#include "Renderer.h"
#include "Ball.h"
#include "vec.h"

Game::Game(const GameParam& param)
    : numberGen(1)
    , colorFrag(0, 255)
{
    config = param;
    renderer = nullptr;
    currColor = config.primaryColor;
    nextColor = randomColor();
    gradient = nextColor - currColor;
    frac = 0.f;
    
    for (int i = 0; i < MULTITOUCH_LIMIT; i++)
    {
        touches[i].interactionMode = DISABLED;
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
    
    frac += dt / 5.f;
    if (frac >= 1.f)
    {
        frac = 0.f;
        currColor = nextColor;
        nextColor = randomColor();
        gradient = nextColor - currColor;
    }
    glm::vec4 primaryColor = currColor + gradient * frac;
    renderer->setUniform("u_primaryColor", primaryColor);
    
    solver.update(dt);
}

void Game::draw()
{
    renderer->loadScene(solver.getObjects());
    renderer->draw();
}

glm::vec4 Game::randomColor()
{
    glm::vec4 c;
    c.x = colorFrag(numberGen) / 255.f;
    c.y = colorFrag(numberGen) / 255.f;
    c.z = colorFrag(numberGen) / 255.f;
    c.w = 1.f;
    return c;
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
        touch->box.width = 2 + numberGen() % 4;
        touch->box.height = 2 + numberGen() % 4;
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
    if (touch->interactionMode == DISABLED)
    {
        return;
    }
    if (touch->interactionMode == NONE)
    {
        detectMode(touch);
    }
    
    if (fullMemory(touch))
    {
        touch->interactionMode = DISABLED;
        return;
    }
    
    Ball* b;
    Finger* finger = &touch->finger;
    switch (touch->interactionMode)
    {
        case MOVE_OBJECT:
            touch->clamp.target->pos = finger->pos;
            break;
            
        case DROP_OBJECT:
            solver.addBox(finger->pos, config.ballRadius, touch->box.width, touch->box.height, PRIMARY_COLOR);
            touch->interactionMode = DISABLED;
            break;
        
        case BUILD_BRIDGE:
            if (touch->bridge.currHead == nullptr)//finger->action == anut::MotionEvent::ACTION_DOWN)
            {
                touch->bridge.currHead = solver.addBall(finger->pos, config.ballRadius, PRIMARY_COLOR, true);
            }
            else if (finger->action == anut::MotionEvent::ACTION_UP)
            {
                b = solver.addBall(finger->pos, config.ballRadius, PRIMARY_COLOR, true);
                solver.link(touch->bridge.currHead, b);
                touch->interactionMode = DISABLED;
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

bool Game::fullMemory(Touch* t)
{
    if (t->interactionMode == DROP_OBJECT)
    {
        int newBalls = t->box.width * t->box.height;
        if (solver.ballCount() + newBalls > config.maxBallCount)
        {
            return true;
        }
    }
    else if (t->interactionMode == BUILD_BRIDGE)
    {
        if (solver.ballCount() + 1 > config.maxBallCount)
        {
            return true;
        }
    }
    return false;
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
            touch->interactionMode = DISABLED;
            touch->unpauseBalls();
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
