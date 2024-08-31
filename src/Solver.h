#ifndef SOLVER_H
#define SOLVER_H
#include "vec.h"
#include "shader.h"
#include "Grid.h"
#include <glm/glm.hpp>
#include <vector>

class Ball;

struct Link
{
    Ball* obj1;
    Ball* obj2;
    float targetDist;
};

class Solver 
{
public:
    Solver();
    
    void init(int maxBallCount, float maxBallRadius);
    void update(float dt);
    Ball* findObject(const vec& p);
    Ball* addBox(const vec& centerPos, float radius, int w, int h, const Color& color);
    Ball* addLine(const vec& centerPos, int w, float radius, const Color& color);
    Ball* addBall(const vec& pos, float radius, const Color& color, bool fix = false);
    void link(Ball* b1, Ball* b2);
    void link(Ball* b1, Ball* b2, float minDist);
    Ball* getLastBall();
    int ballCount() const;
    const std::vector<Ball>& getObjects() const;
    
private:
    void applyGravity(float dt);
    void applyLink();
    void applyCollision();
    void constrain(Ball* b);
    
    std::vector<Ball> objects;
    std::vector<Link> links;
    glm::vec2 screenSize;
    Grid collisionGrid;
    int ballCapacity;
    int nextBallIndex;
};

inline const std::vector<Ball>& Solver::getObjects() const
{
    return objects;
}

#endif
