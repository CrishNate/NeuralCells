#include "Body.h"

#define MOVE_ELEMENT "move_element";

class MoveElement : public Body
{
public:

	MoveElement(Body* pParent, Vector2D offset, int scale)
		: Body(pParent, offset, scale)
		, m_Speed(0)
	{
		Vector2D dir = pParent->GetPosition() - m_Position;
		dir.Normalize();
		m_AngleOffset = std::atan2(dir.y, dir.x);
	}

	virtual void Draw(RGBColor color, float angle, const Camera& camera)
	{
		float dTime = (clock() - m_LTime);
		m_LTime = clock();

		angle += m_AngleOffset - M_PI / 2;
		m_Swim += m_Speed / 50.0f * dTime;

		if (m_Swim > M_PI * 2)
			m_Swim -= M_PI * 2;
		else if (m_Swim < M_PI * 2)
			m_Swim += M_PI * 2;

		Vector2D right = Vector2D(1, 0).GetRotated(angle) * m_Scale / 2;
		Vector2D forward = Vector2D(0, -1).GetRotated(angle) * m_Scale / 3;

		std::vector<Vector2D> points = { m_Position };
		//Vector2D lastPos = m_Position;
		for (int i = 0; i < 10; i++)
		{
			Vector2D pos = right * cos(i * M_PI / 4 - m_Swim) * fmin(1, (i + 1) / 4.0f) + forward * i + m_Position;
			//DrawLineThinkT(
			//	lastPos.x, lastPos.y, 
			//	pos.x, pos.y, 
			//	(m_Scale / 4) * (10 - i) / 10.0f, 
			//	(m_Scale / 4) * (10 - (i + 1)) / 10.0f, 
			//	color, camera
			//);

			//lastPos = pos;

			points.push_back(pos);
		}
		DrawLinesThinkT(points, m_Scale / 4, 0, color, camera);
	}

	Vector2D GetDir(float angle)
	{
		return -m_Offset.GetRotated(angle);
	}

	void SetSpeed(float speed) { m_Speed = speed; }
	float GetSpeed() { return m_Speed; }

	virtual std::string GetType() { return MOVE_ELEMENT; }

private:
	float m_Swim;
	float m_AngleOffset;
	float m_Speed;

	time_t m_LTime;
};