#ifndef GRID_H
#define GRID_H
#include "vec.h"
#include <vector>

class Ball;

class Grid
{
public:
	using Quad = std::vector<Ball*>;
	
	Grid();
	~Grid();
	
	void config(const vec& size, const vec& quadDimension);
	void loadObjects(std::vector<Ball>& objects);
	void detectCollisions();
	Ball* findObject(const vec& point);
	
private:
	Quad* quad(int row, int col);
	Quad* quad(const vec& point);
	void loadAdjacentCells(int row, int col, std::vector<Quad*>& dest);
	void collide(Ball* target, std::vector<Quad*>& neighbors);
	void collide(Ball* b1, Ball* b2);
	bool hasCollision(const Ball& b1, const Ball& b2);
	bool hasCollision(const Ball& ball, const vec& pos);
	
	std::vector<Quad> quads;
	std::vector<Quad*> neighborCellsCache;
	vec gridPos;
	vec quadSize;
	vec gridSize;
	int colCount;
	int rowCount;
};

#endif
