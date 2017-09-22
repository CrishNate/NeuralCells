#include "World.h"


bool World::AddMeat()
{
	int itt = 0;
	while (true)
	{
		bool brk = true;
		float scale = frand(20, 30);
		Vector2D pos = Vector2D(rand() % WORLD_SIZE_X, rand() % WORLD_SIZE_Y);
		for (Meat* pMeat : m_Meat)
		{
			if (pos.Distance(pMeat->GetPosition()) < 800)
			{
				brk = false;
				break;
			}
		}

		if (brk)
		{
			Meat* pMeat = new Meat(pos, scale);
			m_Meat.push_back(pMeat);
			//m_PhysObjs.push_back(pMeat);
			break;
		}

		itt++;
		if (itt > 1000)
			return false;
	}
	return true;
}


bool World::AddPlantGenerator()
{
	int itt = 0;
	while (true)
	{
		bool brk = true;
		float scale = frand(50, 200);
		float x = frand(scale + MAX_PLANT_SIZE * 2, WORLD_SIZE_X - scale - MAX_PLANT_SIZE * 2);
		float y = frand(scale + MAX_PLANT_SIZE * 2, WORLD_SIZE_Y - scale - MAX_PLANT_SIZE * 2);
		Vector2D pos = Vector2D(x, y);
		for (PlantGenerator* pPG : m_PlantGenerators)
		{
			if (pos.Distance(pPG->GetPosition()) < (pPG->GetScale() + scale) * 2 + 1000)
			{
				brk = false;
				break;
			}
		}

		if (brk)
		{
			PlantGenerator* pPG = new PlantGenerator(pos, scale);
			m_PlantGenerators.push_back(pPG);
			//m_PhysObjs.push_back(pPG);
			break;
		}

		itt++;
		if (itt > 1000)
			return false;
	}
	return true;
}


void World::Draw(const Camera& camera)
{
	// Drawing Meat
	for (Meat* pMeat : m_Meat)
	{
		pMeat->Draw(camera);
	}

	// Drawing Plant
	for (Plant* pPlant : m_Plants)
	{
		pPlant->Draw(camera);
	}

	// Drawing Plant Generator
	for (PlantGenerator* pPGenerator : m_PlantGenerators)
	{
		pPGenerator->Draw(camera);
	}

	// Drawing Cell Creature
	for (CellCreature* pCCreature : m_CellCreatures)
	{
		pCCreature->Draw(camera);
	}
}


void World::Step()
{
	// Handling Cell Creature
	for (int i = 0; i < m_CellCreatures.size(); i++)
	{
		CellCreature* pCCreature = m_CellCreatures[i];
		pCCreature->Run(this);
	}

	// Handling Cell Creature
	for (Meat* pMeat : m_Meat)
	{
		pMeat->Run(this);
	}

	// Handling Cell Creature
	for (PlantGenerator* pPGenerator : m_PlantGenerators)
	{
		Plant* pPlant = pPGenerator->AddPlant();
		if (pPlant)
			m_Plants.push_back(pPlant);
	}
}