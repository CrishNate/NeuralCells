#include "Plant.h"
#include "PlantGenerator.h"

void Plant::Remove()
{
	m_pGenerator->Remove(this);
	delete this;
}