#pragma once
#include <vector>
#include "math.h"
#include "Drawing.h"

#define BODY "body";

class Body
{
public:
	Body(Vector2D pos, int scale, float childProbability)
		: m_Scale(scale)
		, m_Position(pos)
		, m_pParent(NULL)
		, m_ChildProbability(childProbability)
	{ }

	Body(Body* pParent, Vector2D offset, int scale, float childProbability)
		: m_Scale(scale)
		, m_pParent(pParent)
		, m_Offset(offset)
		, m_ChildProbability(childProbability)
		, m_Position(pParent->GetPosition() + offset)
	{ }

	Body(Body* pParent, Vector2D offset, int scale)
		: m_Scale(scale)
		, m_pParent(pParent)
		, m_Offset(offset)
		, m_Position(pParent->GetPosition() + offset)
	{ }

	void AddChild(Body* pChild)
	{
		m_Childs.push_back(pChild);
	}

	virtual void Draw(RGBColor color, float angle, const Camera& camera)
	{
		DrawFilledCircle(m_Position.x, m_Position.y, m_Scale, color, camera);
	}

	void MoveToParent(float angle)
	{
		if (m_pParent)
			m_Position = m_pParent->GetPosition() + m_Offset.GetRotated(angle);
	}

	void SetPosition(Vector2D pos) { m_Position = pos; }
	Vector2D GetPosition() { return m_Position; }
	Vector2D GetOffset() { return m_Offset; }
	int GetScale() { return m_Scale; }
	float GetChildProbability() { return m_ChildProbability; }
	void SetChildProbability(float childProbability) { m_ChildProbability = childProbability; }
	std::vector<Body*>& GetChilds() { return m_Childs; }
	
	virtual std::string GetType() { return BODY; }

protected:
	// fields
	Vector2D m_Position;
	Vector2D m_Offset;
	int m_Scale;
	Body* m_pParent;
	std::vector<Body*> m_Childs;
	float m_ChildProbability;
};