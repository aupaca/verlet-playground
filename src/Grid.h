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
	void handleCollision(std::vector<Ball>& objects);
	
private:
	void mapObjects(std::vector<Ball>& objects);
	Quad* quad(int row, int col);
	Quad* quad(const vec& point);
	void clear();
	void collide(Ball& obj);
	void collide(Ball& obj, Quad& nearQuad);
	
	std::vector<Quad> quads;
	vec gridPos;
	vec quadSize;
	vec gridSize;
	float colCount;
	float rowCount;
};

#endif
