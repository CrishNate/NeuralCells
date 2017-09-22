#include "PlantGenerator.h"

PlantGenerator::PlantGenerator(const Vector2D& pos, int scale)
	: PhysObj(pos, scale)
	, m_Scale(scale)
{
	SetStatic();
}