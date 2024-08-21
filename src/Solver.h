#ifndef SOLVER_H
#define SOLVER_H
#include "vec.h"
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
    Ball* findObjectAtPoint(const vec& p);
    Ball* addBall(const vec& pos, float radius, const glm::vec4& color, bool fix = false);
    Ball* addBox(const vec& centerPos, float radius, int w, int h, const glm::vec4& color);
    Ball* addLine(const vec& centerPos, int w, float radius, const glm::vec4& color);
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
