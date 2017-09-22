#pragma once
#include "NeuralNetwork.h"
#include "math.h"
#include "PhysObj.h"

#define START_CHILD_BODYPART_POSSIBILITY 90
#define CHILD_DIVIDE_POSSIBILITY_COEF 2

class Body;
class Mouth;
class MoveElement;
//class Eye;
class Plant;
class Meat;
class Spike;
class World;

enum BodyElements
{
	BE_Undef = 0,
	BE_Mouth = 1,
	//BE_Eyes = 2,
	BE_MoveElement = 2,
	//BE_Spike = 4,
	BE_Count,
};

class CellCreature : public PhysObj
{
public:
	CellCreature(Vector2D pos);

	void Draw(const Camera& camera);
	void DrawP(const Camera& camera, Vector2D pos);

	void Run(World* pWorld);

	float GetRadius() { return m_Radius; }
	NeuralNetwork* GetBrain() { return m_pBrain; }
	void TakeDamage(float damage, World* pWorld);

	float GetHealth() { return m_Health; }

	void SetHunger(float food) { m_Hunger += food; }
	float GetHunger() { return m_Hunger; }

	void Remove()
	{
		delete this;
	}

private:

	bool AddSubBody(Body* pBodypart, std::vector<Body*>& bodies);
	bool AddModifier(Body* pBodypart, std::vector<Body*>& bodies, BodyElements bodyelement);

	// Recursive draw
	void DrawR(Body* pBody, const Camera& camera);

	// fields
	Vector2D m_ModelOffset;
	Body* m_Mainbody;
	RGBColor m_Color;

	float m_Hunger;
	float m_Health;

	std::vector<Mouth*> m_Mouth;
	//std::vector<Eye*> m_Eyes;
	std::vector<Spike*> m_Spikes;
	std::vector<MoveElement*> m_MoveElements;

	NeuralNetwork* m_pBrain;
};