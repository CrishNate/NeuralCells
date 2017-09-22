#include "Body.h"

#define EYE "eye"
#define EYES_VISION_DISTANCE 500

class Plant;
class CellCreature;

class Eye : public Body
{
public:

	Eye(Body* pParent, Vector2D offset, int scale, float fow)
		: Body(pParent, offset, scale)
		, m_FOW(M_PI / 20)
	{
		Vector2D dir = pParent->GetPosition() - m_Position;
		dir.Normalize();
	}

	virtual void Draw(RGBColor color, float angle, const Camera& camera)
	{
		DrawFilledCircle(m_Position.x, m_Position.y, m_Scale, RGBColor(255, 255, 255), camera);
		DrawFilledCircle(m_Position.x, m_Position.y, m_Scale / 3, RGBColor(0, 0, 0), camera);
	}

	Vector2D GetDir(float angle)
	{
		return m_Offset.GetRotated(angle);
	}

	virtual std::string GetType() { return EYE; }

private:

	float m_FOW;
};