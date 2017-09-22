#include "CellCreature.h"
#include "Body.h"
#include "Mouth.h"
#include "MoveElement.h"
#include "Eye.h"
#include "Shape.h"
#include "Spike.h"
#include "World.h"

#define ADDICTIONAL_INPUT_NEURONS_FOR_RANDOM_NOISE 5

CellCreature::CellCreature(Vector2D pos)
	: PhysObj(pos, 0)
	, m_Color(0, 0, 0)
	, m_Health(100)
	, m_Hunger(100)
{
	float scale = rand() % 20 + 10;

	HSL hsl = HSL(frand(0, 400), 0.5f + frand(0, 1) / 2.0f, 0.5f);
	m_Color = HSLToRGB(hsl);

	std::vector<Body*> bodies;
	m_Mainbody = new Body(pos, scale, START_CHILD_BODYPART_POSSIBILITY);
	bodies.push_back(m_Mainbody);

	AddSubBody(m_Mainbody, bodies);
	int bodiesCount = bodies.size();

	Vector2D center = Vector2D(0, 0);
	for (Body* pBody : bodies)
	{
		center += pBody->GetOffset();
	}
	center /= bodies.size();
	m_ModelOffset = center;

	// Getting radius by mass center
	Vector2D centerW = m_Position + m_ModelOffset;
	m_Radius = scale;

	for (Body* pBody : bodies)
	{
		// Getting max radius
		if (m_Radius < (pBody->GetPosition().Distance(centerW) + pBody->GetScale()))
			m_Radius = (pBody->GetPosition().Distance(centerW) + pBody->GetScale());
	}

	// Adding modifiers
	for (int i = 0; i < (rand() % (bodiesCount * 10) + 10); i++)
	{
		int rvel = rand() % bodiesCount;
		AddModifier(bodies[rvel], bodies, (BodyElements)(rand() % (BE_Count - 1) + 1));
	}

	// creating brain
	int inputNeuronsCount = m_Mouth.size() + ADDICTIONAL_INPUT_NEURONS_FOR_RANDOM_NOISE;
	int outputNeuronsCount = m_MoveElements.size();

	std::vector<int> layers = { inputNeuronsCount, 8, outputNeuronsCount };
	m_pBrain = new NeuralNetwork(10, layers);
}

void CellCreature::Draw(const Camera& camera)
{
	m_Mainbody->SetPosition(m_Position - m_ModelOffset.GetRotated(m_Orient));
	DrawR(m_Mainbody, camera);

}

void CellCreature::DrawP(const Camera& camera, Vector2D pos)
{
	m_Mainbody->SetPosition(pos - m_ModelOffset.GetRotated(m_Orient));
	DrawR(m_Mainbody, camera);

}
void CellCreature::Run(World* pWorld)
{
	const std::vector<Plant*>& plants = pWorld->GetPlants();
	const std::vector<Meat*>& meats = pWorld->GetMeat();
	const std::vector<CellCreature*>& cellCreaturs = pWorld->GetCellCreatures();

	int pos = 0;
	std::vector<float> inputs = { 
		frand(1, -1), 
		1,
		-1,
		static_cast<float>(cos(clock() / 400.0f)),
		static_cast<float>(sin(clock() / 600.0f)) 
	};

	pos = inputs.size();

	for (int i = 0; i < m_Mouth.size(); i++)
	{
		inputs.resize(inputs.size() + 1);
		Mouth* pMouth = m_Mouth[i];

		int side = 0;
		pMouth->GetMouthData(plants, meats, cellCreaturs, this, m_Orient, 400, side);

		// For Meat type
		if (pMouth->GetConsumeType() == FoodType::FT_Meat)
		{
			if (PhysObj* pFood = pMouth->CanConsumeFood(this, meats, cellCreaturs))
			{
				if (CellCreature* pCCreature = dynamic_cast<CellCreature*>(pFood))
				{
					Vector2D dir = (pFood->GetPosition() - m_Position).GetNormalize();
					pCCreature->TakeDamage(pMouth->GetScale(), pWorld);
					pCCreature->ApplyForce(dir * 5.0f);
					ApplyForce(-dir * 5.0f);

					pMouth->ConsumeFood(pCCreature, pWorld);
				}
				else if (Meat* pMeat = dynamic_cast<Meat*>(pFood))
				{
					m_Hunger += 25.0f;
					pMouth->ConsumeFood(pMeat, pWorld);
				}
			}
		}
		// For Plant type
		else if (pMouth->GetConsumeType() == FoodType::FT_Plant)
		{
			if (PhysObj* pFood = pMouth->CanConsumeFood(plants))
			{
				if (Plant* pPlant = dynamic_cast<Plant*>(pFood))
				{
					m_Hunger += 5.0f;
					pMouth->ConsumeFood(pPlant, pWorld);
				}
			}
		}

		inputs[pos + i] = side;
	}

	std::vector<float> outputs = m_pBrain->Run(inputs);

	for (int i = 0; i < m_MoveElements.size(); i++)
	{
		MoveElement* pMElem = m_MoveElements[i];
		pMElem->SetSpeed(outputs[i]);

		float speed = pMElem->GetSpeed();
		float scale = pMElem->GetScale();
		Vector2D dirC = (pMElem->GetPosition() - m_Position); // dir to center
		Vector2D dirF = pMElem->GetDir(m_Orient).GetNormalize(); // force dir normalized
		//m_AngularVelocity += Cross(dirC, dirF) * scale * speed / 50.0f;
		//m_Velocity += dirF * scale * speed / 1.0f;

		ApplyImpulse(dirF * speed * scale / 400.0f, dirC / 1000.0f);
	}

	RunPhys();

	// width
	if (m_Position.x < 0)
		m_Position.x += pWorld->width();
	else if (m_Position.x > pWorld->width())
		m_Position.x -= pWorld->width();
	// height
	else if (m_Position.y < 0)
		m_Position.y += pWorld->height();
	else if (m_Position.y > pWorld->height())
		m_Position.y -= pWorld->height();

	// Friction
	m_Velocity /= 1.1f;
	m_AngularVelocity /= 1.1f;
}

void CellCreature::TakeDamage(float damage, World* pWorld)
{
	m_Health -= damage;

	if (m_Health <= 0)
	{
		for (int i = 0; i < 4; i++)
		{
			Vector2D dir = Vector2D(frand(-1, 1), frand(-1, 1)).GetNormalize() * frand(1, 10);
			pWorld->AddMeat(m_Position, dir);
		}

		pWorld->Remove(this);
	}
}

bool CellCreature::AddSubBody(Body* pBodypart, std::vector<Body*>& bodies)
{
	float coef = pBodypart->GetChildProbability();
	if (frand(0, 100) > coef)
		return false;

	int itt = 0;
	while (true)
	{
		bool correct = true;
		float val = frand(0, M_PI * 2);
		float scale = rand() % 20 + 10;
		Vector2D offset = Vector2D(cos(val), sin(val)) * (scale + pBodypart->GetScale()) / 1.1f;
		Vector2D pos = pBodypart->GetPosition() + offset;
		// break if too much itterations
		itt++;
		if (itt > 1000)
			return false;

		for (Body* pBody : bodies)
		{
			if (pBody->GetPosition().Distance(pos) < (scale + pBody->GetScale()) / 1.1f)
			{
				correct = false;
				break;
			}
		}

		if (correct)
		{
			float coef = pBodypart->GetChildProbability() / CHILD_DIVIDE_POSSIBILITY_COEF;
			Body* pBody = new Body(pBodypart, offset, scale, coef);
			pBodypart->AddChild(pBody);
			bodies.push_back(pBody);
			pBodypart->SetChildProbability(coef);

			if (frand(0, 100) < coef)
				AddSubBody(pBody, bodies);

			if (AddSubBody(pBodypart, bodies))
				pBodypart->SetChildProbability(coef);

			break;
		}
	}

	return true;
}

bool CellCreature::AddModifier(Body* pBodypart, std::vector<Body*>& bodies, BodyElements bodyelement)
{
	int itt = 0;
	while (true)
	{
		bool correct = true;
		float val = frand(0, M_PI * 2);
		float scale = 0;
		float modelScale = 0;
		float randCoef = (frand(0, pBodypart->GetScale() - 10) + 10);
		Vector2D offset = Vector2D(cos(val), sin(val)) * pBodypart->GetScale() / 1.1f;

		switch (bodyelement)
		{
		case BE_Mouth:
			scale = randCoef / 1.2;
			modelScale = scale / 1.2;
			break;
		//case BE_Eyes:
		//	scale = randCoef / 2;
		//	modelScale = scale;
		//	offset /= 4;
		//	break;
		case BE_MoveElement:
			scale = randCoef / 4;
			modelScale = scale * 4;
			break;
		//case BE_Spike:
		//	scale = randCoef / 4;
		//	modelScale = scale * 4;
		//	break;
		default:
			scale = randCoef;
			modelScale = scale;
			break;
		}
		Vector2D pos = pBodypart->GetPosition() + offset;

		// break if too much itterations
		itt++;
		if (itt > 1000)
			return false;

		for (Body* pBody : bodies)
		{
			if (pBody != pBodypart && pBody->GetPosition().Distance(pos) < (scale + pBody->GetScale()) / 1.1f)
			{
				correct = false;
				break;
			}
		}

		if (correct)
		{
			Body* pBody = NULL;
			if (bodyelement == BodyElements::BE_Mouth)
			{
				pBody = new Mouth(pBodypart, offset, modelScale, (FoodType)(rand() % 2 + 1));
				m_Mouth.push_back(dynamic_cast<Mouth*>(pBody));
			}
			else if (bodyelement == BodyElements::BE_MoveElement)
			{
				pBody = new MoveElement(pBodypart, offset, modelScale);
				m_MoveElements.push_back(dynamic_cast<MoveElement*>(pBody));
			}
		/*	else if (bodyelement == BodyElements::BE_Eyes)
			{
				pBody = new Eye(pBodypart, offset, modelScale, frand(M_PI / 20, M_PI / 10));
				m_Eyes.push_back(dynamic_cast<Eye*>(pBody));
			}*/
			//else if (bodyelement == BodyElements::BE_Spike)
			//{
			//	pBody = new Spike(pBodypart, offset, modelScale);
			//	m_Spikes.push_back(dynamic_cast<Spike*>(pBody));
			//}

			if (pBody)
			{
				pBodypart->AddChild(pBody);
				bodies.push_back(pBody);
			}

			break;
		}
	}

	return true;
}

void CellCreature::DrawR(Body* pBody, const Camera& camera)
{
	pBody->MoveToParent(m_Orient);
	pBody->Draw(m_Color, m_Orient, camera);

	std::vector<Body*>& childs = pBody->GetChilds();

	for (Body* pChild : childs)
	{
		DrawR(pChild, camera);
	}
}
