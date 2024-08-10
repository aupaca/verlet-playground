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
    void addBall(const vec& pos, float radius, const glm::vec4& color, bool fix = false);
    void link(Ball* ball1, Ball* ball2);
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
