#include "Meat.h"
#include "World.h"

void Meat::Run(World* pWorld)
{
	RunPhys();

	m_Velocity /= 1.1;
	// width
	if (m_Position.x < 0)
		m_Position.x += pWorld->width();
	else if (m_Position.x > pWorld->width())
		m_Position.x -= pWorld->width();
	// height
	else if (m_Position.y < 0)
		m_Position.y += pWorld->height();
	else if (m_Position.y > pWorld->height())
		m_Position.y -= pWorld->height();

}