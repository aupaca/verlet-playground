#ifndef GAME_H
#define GAME_H
#include "GameParam.h"
#include "Solver.h"
#include "Finger.h"
#include <ANUT/ANUT.h>
#include <glm/glm.hpp>
#include <random>

class Renderer;
class Ball;

enum DropMode
{
    NONE = 0,
    DROP,
    BRIDGE
};

struct Touch 
{
    void pauseBall(Ball* b);
    void unpauseBalls();
    
    Finger finger;
    int dropMode;
    Ball* ballTarget;
    std::vector<Ball*> tempStoppedBalls;
};

class Game 
{
public:
    Game(const GameParam& param);
    ~Game();
    
    bool initGraphics();
    void deinitGraphics();
    void update(float dt);
    void draw();
    void handleTouch(const anut::MotionEvent& motion);
    
private:
    void dropBall(Touch* t);
    void detectMode(Touch* t);
    
    static constexpr int MULTITOUCH_LIMIT = 5;
    Touch touches[MULTITOUCH_LIMIT];
    GameParam config;
    Renderer* renderer;
    std::mt19937 numberGen;
    std::uniform_int_distribution<> radiusRange;
    Solver solver;
};

#endif
