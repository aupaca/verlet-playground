#ifndef FINGER_H
#define FINGER_H
#include <ANUT/ANUT.h>
#include <ANUT/ANUT_utils.h>
#include <glm/glm.hpp>

struct Finger 
{
    Finger();
    
    bool isPressed() const;
    bool moved(float pixels, const glm::vec2& relativePos);
    float pressedTime() const;
    void onDown(const anut::MotionEvent& motion);
    void onMove(const anut::MotionEvent& motion);
    void onUp(const anut::MotionEvent& motion);
    
    glm::vec2 pos;
    glm::vec2 firstPos;
    anut::Timer::TimePoint downTime;
    int action;
    bool isDown;
};

inline bool Finger::isPressed() const
{
    return isDown;
}

#endif
