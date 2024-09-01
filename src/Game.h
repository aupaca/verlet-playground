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

enum Mode
{
    NONE = 0,
    DROP_OBJECT,
    BUILD_BRIDGE,
    MOVE_OBJECT
};

struct Touch 
{
    void pauseBall(Ball* b);
    void unpauseBalls();
    
    Finger finger;
    int interactionMode;
    std::vector<Ball*> tempStoppedBalls;
    bool canceled;
    
    union
    {
        struct
        {
            Ball* currHead;
            bool finalize;
        } bridge;
        
        struct
        {
            int width;
            int height;
        } box;
        
        struct
        {
            Ball* target;
        } clamp;
    };
};

class Game 
{
public:
    Game(const GameParam& param);
    ~Game();
    
    bool init();
    bool initGraphics();
    void deinitGraphics();
    void update(float dt);
    void draw();
    void handleTouch(const anut::MotionEvent& motion);
    
private:
    void interact(Touch* t);
    void detectMode(Touch* t);
    bool hasCapacity(int ballsToAdd);
    
    static constexpr int MULTITOUCH_LIMIT = 5;
    Touch touches[MULTITOUCH_LIMIT];
    GameParam config;
    Renderer* renderer;
    std::mt19937 numberGen;
    std::uniform_int_distribution<int> boxWidth;
    std::uniform_int_distribution<int> boxHeight;
    Solver solver;
};

#endif
