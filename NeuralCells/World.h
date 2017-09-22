#pragma once

#include "Meat.h"
#include "Plant.h"
#include "PlantGenerator.h"
#include "CellCreature.h"

#define WORLD_SIZE_X 4000
#define WORLD_SIZE_Y 4000
#define CELLCREATURES_COUNT 50


class World
{
public:

	World()
	{
		FillWithPlantGenerators();
		FillWithMeat();
		FillCellCreature();
	}

	bool AddPlantGenerator();

	bool AddMeat();

	void AddMeat(Vector2D pos, Vector2D vel)
	{
		float scale = frand(20, 30);
		Meat* pMeat = new Meat(pos, scale);
		pMeat->ApplyForce(vel);
		m_Meat.push_back(pMeat);
	}

	// Filling functions
	void FillWithPlantGenerators()
	{
		while (true)
		{
			if (!AddPlantGenerator())
				break;
		}
	}

	void FillWithMeat()
	{
		while (true)
		{
			if (!AddMeat())
				break;
		}
	}

	void FillCellCreature()
	{
		for (int i = 0; i < CELLCREATURES_COUNT; i++)
			AddCellCreature();
	}

	// Adding Random Cell Creature to the World
	CellCreature* AddCellCreature()
	{
		Vector2D pos = Vector2D(rand() % WORLD_SIZE_X, rand() % WORLD_SIZE_Y);
		CellCreature* pCreture = new CellCreature(pos);
		m_CellCreatures.push_back(pCreture);
		//m_PhysObjs.push_back(pCreture);
		return pCreture;
	}

	// Size
	float width() { return WORLD_SIZE_X; }
	float height() { return WORLD_SIZE_Y; }

	// Removing functions
	void Remove(Plant* pPlant)
	{
		m_Plants.erase(std::remove(m_Plants.begin(), m_Plants.end(), pPlant), m_Plants.end());
		pPlant->Remove();
	}

	void Remove(Meat* pMeat)
	{
		m_Meat.erase(std::remove(m_Meat.begin(), m_Meat.end(), pMeat), m_Meat.end());
		pMeat->Remove();
	}

	void Remove(CellCreature* pCCreature)
	{
		m_CellCreatures.erase(std::remove(m_CellCreatures.begin(), m_CellCreatures.end(), pCCreature), m_CellCreatures.end());
		pCCreature->Remove();
	}

	const std::vector<Plant*>& GetPlants() { return m_Plants; }

	const std::vector<Meat*>& GetMeat() { return m_Meat; }

	const std::vector<CellCreature*>& GetCellCreatures() { return m_CellCreatures; }

	void Draw(const Camera& camera);

	void Step();

private:

	// fields
	std::vector<Plant*> m_Plants;
	std::vector<Meat*> m_Meat;
	std::vector<PlantGenerator*> m_PlantGenerators;
	std::vector<CellCreature*> m_CellCreatures;
};