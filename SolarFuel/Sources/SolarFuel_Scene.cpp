#include "..\Headers\SolarFuel.hpp"

float SpUp = 0.0f,SpDown = 0.0f, SpRight = 0.0f, SpLeft = 0.0f;

static uint32_t nLehmer = 0;

static uint32_t Lehmer32()
{
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}



void SolarFuel::Scene::Entity::GenerateSystem(const glm::vec2& _CameraPosition, const float _Width, const float _Height)
{
	float SpatiuIntreGriduri = 30;

	float BeginX = (float)(_CameraPosition.x - _Width / 2) - floor((_CameraPosition.x - _Width / 2) / SpatiuIntreGriduri) * SpatiuIntreGriduri;
	float BeginY = (float)(_CameraPosition.y - _Height / 2) - floor((_CameraPosition.y - _Height / 2) / SpatiuIntreGriduri) * SpatiuIntreGriduri;

	for (float j = BeginY; j <= _CameraPosition.y + _Height / 2; j += SpatiuIntreGriduri)
	{
		for (float i = BeginX; i <= _CameraPosition.x + _Width / 2; i += SpatiuIntreGriduri)
		{
			float posX = i;
			float posY = j;
			nLehmer = ((int)(posY / SpatiuIntreGriduri)) << 20 | ((int)(posX / SpatiuIntreGriduri));
			if (Lehmer32() % 256 < 32) {
				Entity* entity = new Entity;
				entity->Parent = this;
				entity->Position = glm::vec2(posX, posY);
				Childs.push_back(entity);
			}
		}
	}
	
	for (auto Sun : Childs)
	{
		srand(Sun->Position.x + Sun->Position.y);
		int NrPlanets = rand() % 7;
		float Dist = (float) SpatiuIntreGriduri / 7;
		for (int i = 0; i < NrPlanets; i++)
		{
			Entity* Planet = new Entity;
			Planet->Parent = Sun;
			Planet->Position = glm::vec2(Dist*(i+1), Sun->Position.y);
			Sun->Childs.push_back(Planet);
			int NrSatelites = rand() % 4;
			float DistSatelites =(float) Dist / 4;
			for (int j = 0; j < NrSatelites; j++)
			{
				Entity* Satelite = new Entity;
				Satelite->Parent = Planet;
				Satelite->Position = glm::vec2(Planet->Position.x, DistSatelites * (j + 1));
				Planet->Childs.push_back(Satelite);
			}
		}
	}
}


void SolarFuel::Scene::Entity::DestroyAllChilds()
{
	for (Entity* _Child : Childs)
	{
		_Child->DestroyAllChilds();
		delete _Child;
	}

	Childs.clear();
}

const glm::mat4 SolarFuel::Scene::Entity::GetLocalMatrix() const
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(Angle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(Scale.x, Scale.y, 1.0f));
}

const glm::mat4 SolarFuel::Scene::Entity::GetWorldMatrix() const
{
	if (!Parent)
	{
		return GetLocalMatrix();
	}

	return Parent->GetWorldMatrix() * GetLocalMatrix();
}
