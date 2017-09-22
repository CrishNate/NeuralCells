#pragma once
#include <vector>
#include "math.h"
#include "Drawing.h"
#include "PhysObj.h"


class World;


class Meat : public PhysObj
{
public:

	Meat(Vector2D pos, int radius)
		: PhysObj(pos, radius)
	{ }

	virtual void Draw(const Camera& camera)
	{
		DrawFilledCircle(m_Position.x, m_Position.y, m_Radius, RGBColor(255, 0, 0), camera);
	}

	void Run(World* pWorld);

	void Remove() 
	{
		delete this;
	}

private:

	// fields
};