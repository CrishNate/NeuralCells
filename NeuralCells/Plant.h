#pragma once
#include <vector>
#include "math.h"
#include "Drawing.h"
#include "PhysObj.h"


class PlantGenerator;

class Plant : public PhysObj
{
public:

	Plant(Vector2D pos, int radius)
		: PhysObj(pos, radius)
	{ }

	virtual void Draw(const Camera& camera)
	{
		DrawFilledCircle(m_Position.x, m_Position.y, m_Radius, RGBColor(0, 255, 0), camera);
	}

	void Remove();

	void SetGenerator(PlantGenerator* pGenerator) { m_pGenerator = pGenerator; }

private:

	// fields
	PlantGenerator* m_pGenerator;
};