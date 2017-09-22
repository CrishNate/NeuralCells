#include "Body.h"
#include "Meat.h"
#include "Plant.h"
#include "CellCreature.h"

#define CONSUME_COOLDOWN 1000

extern Camera g_Camera;

class World;

enum FoodType
{
	FT_Undef = 0,
	FT_Plant = 1,
	FT_Meat = 2,
};

class Plant;
class Meat;
class CellCreature;

class Mouth : public Body
{
public:

	Mouth(Body* pParent, Vector2D offset, int scale, FoodType consumeType)
		: Body(pParent, offset, scale)
		, m_ConsumeType(consumeType)
	{
		Vector2D dir = pParent->GetPosition() - m_Position;
		dir.Normalize();
		m_AngleOffset = std::atan2(dir.y, dir.x);
	}

	virtual void Draw(RGBColor color, float angle, const Camera& camera);

	PhysObj* CanConsumeFood(CellCreature* pCellCreature, const std::vector<Meat*>& meats, const std::vector<CellCreature*>& cellCreaturs);
	PhysObj* CanConsumeFood(const std::vector<Plant*>& plants);

	void ConsumeFood(Meat* pMeat, World* pWorld);
	void ConsumeFood(Plant* pMeat, World* pWorld);
	void ConsumeFood(CellCreature* pCCreature, World* pWorld);

	void GetMouthData(const std::vector<Plant*>& plants, const std::vector<Meat*>& meats, const std::vector<CellCreature*>& cellCreaturs
		, CellCreature* pCellIgnore, float angle, float viewRadius, int& side);

	FoodType GetConsumeType() { return m_ConsumeType; }

private:

	bool GetClosestMeat(const std::vector<Meat*>& meats, const std::vector<CellCreature*>& cellcreatures, CellCreature* pCellIgnore, float angle, float viewRadius, int& side);

	bool GetClosestPlant(const std::vector<Plant*>& plants, float angle, float viewRadius, int& side);

	// fields
	FoodType m_ConsumeType;
	float m_Chum;
	float m_AngleOffset;
	time_t m_ConsumeCooldown;
};