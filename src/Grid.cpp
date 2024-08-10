#include "Grid.h"
#include "Ball.h"
#include <cmath>

Grid::Grid()
{
	
}

Grid::~Grid()
{
	
}

void Grid::config(const vec& size, const vec& quadDimension)
{
	gridSize = size;
	gridPos = vec(0.f, 0.f);
	quadSize = quadDimension;
	rowCount = ceil(gridSize.y / quadSize.y) + 1;
	colCount = ceil(gridSize.x / quadSize.x) + 1;
	quads.resize(rowCount * colCount);
}

void Grid::handleCollision(std::vector<Ball>& objects)
{
    mapObjects(objects);
    for (Ball& ball : objects)
    {
        collide(ball);
    }
}

void Grid::mapObjects(std::vector<Ball>& objects)
{
	clear();
	for (Ball& b : objects)
	{
		Quad* cell = quad(b.pos);
		if (cell)
		{
			cell->push_back(&b);
		}
	}
}

void Grid::collide(Ball& obj)
{
	int row = obj.pos.y / quadSize.y;
	int col = obj.pos.x / quadSize.x;
	
	int leftQuad = col == 0 ? col : col - 1;
	int rightQuad = col + 1 == colCount ? col : col + 1;
	int topQuad = row == 0 ? row : row - 1;
	int bottomQuad = row + 1 == rowCount ? row : row + 1;
	
	for (int i = topQuad; i < bottomQuad; i++)
	{
		for (int j = leftQuad; j < rightQuad; j++)
		{
			Quad* nearObjects = quad(i, j);
			collide(obj, *nearObjects);
		}
	}
}

void Grid::collide(Ball& obj, Quad& group)
{
	for (Ball* b : group)
	{
		Ball& otherObj = *b;
		if (&obj == &otherObj)
		{
			return;
		}
		vec axis = obj.pos - otherObj.pos;
		float dist2 = axis.x * axis.x + axis.y * axis.y;
		float targetDist = obj.radius + otherObj.radius;
		if (dist2 < targetDist * targetDist)
		{
			setDistance(&obj, &otherObj, targetDist);
		}
	}
}

Grid::Quad* Grid::quad(int row, int col)
{
	if (row < 0 || row >= rowCount)
	{
		return nullptr;
	}
	if (col < 0 || col >= colCount)
	{
		return nullptr;
	}
	int index = row * colCount + col;
	return &quads[index];
}

Grid::Quad* Grid::quad(const vec& point)
{
	if (point.x < gridPos.x || point.x > gridPos.x + gridSize.x)
	{
		return nullptr;
	}
	if (point.y < gridPos.y || point.y > gridPos.y + gridSize.y)
	{
		return nullptr;
	}
	int row = point.y / quadSize.y;
	int col = point.x / quadSize.x;
	int index = row * colCount + col;
	return &quads[index];
}

void Grid::clear()
{
	for (auto& q : quads)
	{
		q.clear();
	}
}
