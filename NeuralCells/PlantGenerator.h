#pragma once
#include "Plant.h"
#include <time.h>
#include "PhysObj.h"

#define RESPAWN_TIME 2000
#define MAX_PLANTS_PER_GENERATOR 10
#define MAX_PLANT_SIZE 30
#define MIN_PLANT_SIZE 10

class PlantGenerator : public PhysObj
{
public:

	PlantGenerator(const Vector2D& pos, int scale);

	virtual void Draw(const Camera& camera)
	{
		DrawFilledCircle(m_Position.x, m_Position.y, m_Scale, RGBColor(0, 255, 0), camera);
	}

	Plant* AddPlant()
	{
		if (m_Block || m_LastSpawn > clock())
			return NULL;

		int itt = 0;
		while (true)
		{
			bool correct = true;
			float val = frand(0, M_PI * 2);
			float scale = frand(MIN_PLANT_SIZE, MAX_PLANT_SIZE);
			Vector2D offset = Vector2D(cos(val), sin(val)) * (scale * 1.5 + m_Scale);
			Vector2D pos = m_Position + offset;

			// break if too much itterations
			itt++;
			if (itt > 1000)
			{
				m_Block = true;
				return NULL;
			}

			for (Plant* pPlant : m_Plants)
			{
				if (pPlant->GetPosition().Distance(pos) < (scale + pPlant->GetRadius()) * 1.5)
				{
					correct = false;
					break;
				}
			}

			if (correct)
			{
				Plant* pPlant = new Plant(pos, scale);
				m_Plants.push_back(pPlant);
				pPlant->SetGenerator(this);

				if (m_Plants.size() > MAX_PLANTS_PER_GENERATOR)
					m_Block = true;

				m_LastSpawn = clock() + RESPAWN_TIME;
				return pPlant;
			}
		}
	}

	void Remove(Plant* pPlant)
	{
		for (int i = 0; i < m_Plants.size(); i++)
		{
			Plant* pPl = m_Plants[i];
			if (pPl == pPlant)
			{
				m_Plants.erase(m_Plants.begin() + i);
				break;
			}
		}

		// droping block
		if (m_Block)
		{
			m_Block = false;
			m_LastSpawn = clock() + RESPAWN_TIME;
		}
	}

	Vector2D GetPosition() { return m_Position; }
	float GetScale() { return m_Scale; }

private:

	float m_Scale;
	std::vector<Plant*> m_Plants;
	bool m_Block;
	time_t m_LastSpawn;
};