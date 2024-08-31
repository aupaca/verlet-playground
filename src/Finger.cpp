#include "Finger.h"

Finger::Finger()
{
    isDown = false;
}

bool Finger::moved(float pixels, const glm::vec2& relativePos)
{
    glm::vec2 delta = relativePos - pos;
    float dist2 = delta.x * delta.x + delta.y * delta.y;
    return dist2 >= pixels * pixels;
}

float Finger::pressedTime() const
{
    return anut::Timer::timeSince(downTime);
}

void Finger::onDown(const anut::MotionEvent& motion)
{
    downTime = anut::Timer::now();
    isDown = true;
    pos = firstPos = {motion.x, motion.y};
    action = anut::MotionEvent::ACTION_DOWN;
}

void Finger::onMove(const anut::MotionEvent& motion)
{
    pos = {motion.x, motion.y};
    action = anut::MotionEvent::ACTION_MOVE;
}

void Finger::onUp(const anut::MotionEvent& motion)
{
    isDown = false;
    pos = {motion.x, motion.y};
    action = anut::MotionEvent::ACTION_UP;
}

