#include "Mouth.h"
#include "Plant.h"
#include "CellCreature.h"
#include "World.h"

void Mouth::Draw(RGBColor color, float angle, const Camera& camera)
{
	angle += m_AngleOffset - M_PI / 2;
	if (m_ConsumeCooldown > clock())
	{
		m_Chum += 0.25;

		if (m_Chum > M_2PI)
			m_Chum -= M_2PI;
	}
	else
	{
		m_Chum = 0;
	}

	if (m_ConsumeType == FoodType::FT_Meat)
	{
		Vector2D left = Vector2D(-1, 0) * m_Scale;
		Vector2D right = Vector2D(1, 0) * m_Scale;
		Vector2D forward = Vector2D(0, 1) * m_Scale;

		Vector2D leftd = (left + forward).GetRotated(angle) + m_Position;
		Vector2D rightd = (right + forward).GetRotated(angle) + m_Position;

		left = left * abs(cos(m_Chum)) * 1.5 + Vector2D(0, -1) * m_Scale;
		right = right * abs(cos(m_Chum)) * 1.5 + Vector2D(0, -1) * m_Scale;
		Vector2D leftt = left.GetRotated(angle) + m_Position;
		Vector2D rightt = right.GetRotated(angle) + m_Position;
		forward.GetRotated(angle);

		DrawTriangle(m_Position.x + forward.x / 10.0f, m_Position.y + forward.y / 10.0f, leftd.x, leftd.y, leftt.x, leftt.y, color, camera);
		DrawTriangle(m_Position.x + forward.x / 10.0f, m_Position.y + forward.y / 10.0f, rightd.x, rightd.y, rightt.x, rightt.y, color, camera);
	}
	else if (m_ConsumeType == FoodType::FT_Plant)
	{
		float anim = cos(m_Chum);
		float anim2 = sin(m_Chum);
		Vector2D leftN = Vector2D(-1 + anim2 / 6.0f, -1).GetNormalize();
		Vector2D rightN = Vector2D(1 - anim2 / 6.0f, -1).GetNormalize();
		Vector2D forwardN = Vector2D(anim / 6.0f, -1).GetNormalize();

		Vector2D leftf = (leftN * m_Scale * 2).GetRotated(angle) + m_Position;
		Vector2D rightf = (rightN * m_Scale * 2).GetRotated(angle) + m_Position;
		Vector2D forward = (forwardN * m_Scale * 2).GetRotated(angle) + m_Position;

		DrawLineThinkT(m_Position.x, m_Position.y, leftf.x, leftf.y, m_Scale / 8, m_Scale / 32, color, camera);
		DrawLineThinkT(m_Position.x, m_Position.y, rightf.x, rightf.y, m_Scale / 8, m_Scale / 32, color, camera);
		DrawLineThinkT(m_Position.x, m_Position.y, forward.x, forward.y, m_Scale / 8, m_Scale / 32, color, camera);
	}
}

void Mouth::ConsumeFood(Meat* pMeat, World* pWorld)
{
	m_ConsumeCooldown = clock() + CONSUME_COOLDOWN;
	pWorld->Remove(pMeat);
}

void Mouth::ConsumeFood(Plant* pPlant, World* pWorld)
{
	m_ConsumeCooldown = clock() + CONSUME_COOLDOWN;
	pWorld->Remove(pPlant);
}

void Mouth::ConsumeFood(CellCreature* pCCreature, World* pWorld)
{
	m_ConsumeCooldown = clock() + CONSUME_COOLDOWN;
}

PhysObj* Mouth::CanConsumeFood(CellCreature* pCellCreature, const std::vector<Meat*>& meats, const std::vector<CellCreature*>& cellCreaturs)
{
	if (m_ConsumeCooldown > clock())
		return NULL;

	float distM = -1;
	PhysObj* pFood = NULL;

	// Finding Closest Meat
	for (Meat* pM : meats)
	{
		float dist = pM->GetPosition().Distance(m_Position);
		if (dist >= 0 && (distM == -1 || distM > dist))
		{
			distM = dist;
			pFood = pM;
		}
	}

	// Finding Closest Cell Creature
	for (CellCreature* pC : cellCreaturs)
	{
		if (pC == pCellCreature)
			continue;

		float dist = pC->GetPosition().Distance(m_Position);
		if (dist >= 0 && (distM == -1 || distM > dist))
		{
			distM = dist;
			pFood = pC;
		}
	}

	if (pFood->GetPosition().Distance(m_Position) > (pFood->GetRadius() + m_Scale))
		return NULL;

	return pFood;
}


PhysObj* Mouth::CanConsumeFood(const std::vector<Plant*>& plants)
{
	if (m_ConsumeCooldown > clock())
		return NULL;

	float distP = -1;
	PhysObj* pFood = NULL;

	// Finding Closest Plant
	for (Plant* pP : plants)
	{
		float dist = pP->GetPosition().Distance(m_Position);
		if (dist >= 0 && (distP == -1 || distP > dist))
		{
			distP = dist;
			pFood = pP;
		}
	}

	if (!pFood)
		return NULL;

	if (pFood->GetPosition().Distance(m_Position) > (pFood->GetRadius() / 2.0f + m_Scale))
		return NULL;

	return pFood;
}


void Mouth::GetMouthData(const std::vector<Plant*>& plants, const std::vector<Meat*>& meats, const std::vector<CellCreature*>& cellCreaturs
	, CellCreature* pCellIgnore, float angle, float viewRadius, int& side)
{
	switch (m_ConsumeType)
	{
	case FT_Meat:
		GetClosestMeat(meats, cellCreaturs, pCellIgnore, angle, viewRadius, side);
		break;
	case FT_Plant:
		GetClosestPlant(plants, angle, viewRadius, side);
		break;
	default:
		break;
	}
}


bool Mouth::GetClosestMeat(const std::vector<Meat*>& meats, const std::vector<CellCreature*>& cellcreatures, CellCreature* pCellIgnore, float angle, float viewRadius, int& side)
{
	float distM = -1;
	PhysObj* pMeat = NULL;

	for (Meat* pM : meats)
	{
		// Plant
		float dist = pM->GetPosition().Distance(m_Position);
		if (dist >= 0 && (distM == -1 || distM > dist))
		{
			distM = dist;
			pMeat = pM;
		}
	}

	for (CellCreature* pC : cellcreatures)
	{
		if (pC == pCellIgnore)
			continue;

		// Plant
		float dist = pC->GetPosition().Distance(m_Position);
		if (dist >= 0 && (distM == -1 || distM > dist))
		{
			distM = dist;
			pMeat = pC;
		}
	}

	if (!pMeat)
		return false;

	Vector2D offset = m_Offset.GetNormalize();
	Vector2D dirF = offset.GetRotated(angle);
	Vector2D dir = (pMeat->GetPosition() - m_Position);
	float ang1 = atan2(dirF.y, dirF.x);
	float ang = ang1 - atan2(dir.y, dir.x);

	if (distM < viewRadius && ang > -M_PI * 0.5f && ang < M_PI * 0.5f)
	{
		if (ang > 0)
		{
			DrawLineThink(m_Position.x, m_Position.y, m_Position.x + dir.x, m_Position.y + dir.y, 1, RGBColor(0, 0, 255), g_Camera);
			side = 1;
		}
		else
		{
			DrawLineThink(m_Position.x, m_Position.y, m_Position.x + dir.x, m_Position.y + dir.y, 1, RGBColor(255, 0, 0), g_Camera);
			side = -1;
		}
	}

	return true;
}

bool Mouth::GetClosestPlant(const std::vector<Plant*>& plants, float angle, float viewRadius, int& side)
{
	float distP = -1;
	Plant* pPlant = NULL;

	for (Plant* pP : plants)
	{
		// Plant
		float dist = pP->GetPosition().Distance(m_Position);
		if (dist >= 0 && (distP == -1 || distP > dist))
		{
			distP = dist;
			pPlant = pP;
		}
	}

	if (!pPlant)
		return false;

	Vector2D offset = m_Offset.GetNormalize();
	Vector2D dirF = offset.GetRotated(angle);
	Vector2D dir = (pPlant->GetPosition() - m_Position);
	float ang1 = atan2(dirF.y, dirF.x);
	float ang = ang1 - atan2(dir.y, dir.x);

	if (distP < viewRadius && ang > -M_PI * 0.5f && ang < M_PI * 0.5f)
	{
		if (ang > 0)
		{
			DrawLineThink(m_Position.x, m_Position.y, m_Position.x + dir.x, m_Position.y + dir.y, 1, RGBColor(0, 0, 255), g_Camera);
			side = 1;
		}
		else
		{
			DrawLineThink(m_Position.x, m_Position.y, m_Position.x + dir.x, m_Position.y + dir.y, 1, RGBColor(255, 0, 0), g_Camera);
			side = -1;
		}
	}

	return true;
}