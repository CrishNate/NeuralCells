#include "Body.h"

#define SPIKE "spike"

class CellCreature;

class Spike : public Body
{
public:

	Spike(Body* pParent, Vector2D offset, int scale)
		: Body(pParent, offset, scale)
	{
		Vector2D dir = pParent->GetPosition() - m_Position;
		dir.Normalize();
		m_AngleOffset = std::atan2(dir.y, dir.x);
	}

	virtual void Draw(RGBColor color, float angle, const Camera& camera)
	{
		angle += m_AngleOffset + M_PI / 2;

		Vector2D left = Vector2D(-0.25, 0) * m_Scale;
		Vector2D right = Vector2D(0.25, 0) * m_Scale;
		Vector2D forward = Vector2D(0, 2) * m_Scale;

		left = left.GetRotated(angle) + m_Position;
		right = right.GetRotated(angle) + m_Position;
		forward = forward.GetRotated(angle) + m_Position;

		Vector2D leftt = left.GetRotated(angle) + m_Position;
		Vector2D rightt = right.GetRotated(angle) + m_Position;

		DrawTriangle(left.x, left.y, right.x, right.y, forward.x, forward.y, RGBColor(200, 200, 200), camera);
	}

	void GetEyeData(const std::vector<CellCreature*>& cellCreaturs)
	{

	}

	virtual std::string GetType() { return SPIKE; }

private:

	float m_FOW;
	Vector2D m_EyeDir;
	float m_AngleOffset;
};