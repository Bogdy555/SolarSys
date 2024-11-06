#include "..\Headers\SolarFuel.hpp"



static uint32_t LehmerSeed = 0;

static uint32_t Lehmer()
{
	LehmerSeed += (uint32_t)(0xE120FC15);
	uint64_t tmp = (uint64_t)(LehmerSeed) * (uint64_t)(0x4A39B70D);
	uint32_t m1 = (uint32_t)((tmp >> 32) ^ tmp);
	tmp = (uint64_t)(m1) * (uint64_t)(0x12FAD5C9);
	uint32_t m2 = (uint32_t)((tmp >> 32) ^ tmp);
	return m2;
}



void SolarFuel::Scene::Entity::GenerateSystem(const glm::vec2& _CameraPosition, const float _Width, const float _Height, const float _ElapsedTime)
{
	DestroyAllChilds();

	if (Parent != nullptr)
	{
		return;
	}

	constexpr float _GridUnit = 10.0f;
	constexpr size_t _MaxPlanetsCount = 10;
	constexpr size_t _MaxSatelitesCount = 10;

	for (float _X = ceil((_CameraPosition.x - _Width / 2.0f) / _GridUnit) * _GridUnit; _X <= _CameraPosition.x + _Width / 2.0f; _X += _GridUnit)
	{
		for (float _Y = ceil((_CameraPosition.y - _Height / 2.0f) / _GridUnit) * _GridUnit; _Y < _CameraPosition.y + _Height / 2.0f; _Y += _GridUnit)
		{
			LehmerSeed = ((uint32_t)(_X / _GridUnit) << 16) + (uint32_t)(_Y / _GridUnit);

			if (Lehmer() % 256 >= 32)
			{
				continue;
			}

			Entity* _Sun = new Entity();

			_Sun->Parent = this;
			_Sun->Position = glm::vec2(_X, _Y);
			_Sun->Scale = glm::vec2(1.0f, 1.0f) * 1.0f;
			_Sun->Angle = 0.0f;
			_Sun->RotationFrequency = 0.0f;

			Childs.push_back(_Sun);

			size_t _PlanetsCount = Lehmer() % _MaxPlanetsCount;

			for (size_t _PlanetIndex = 0; _PlanetIndex < _PlanetsCount; _PlanetIndex++)
			{
				Entity* _Planet = new Entity();

				_Planet->Parent = _Sun;
				_Planet->Position = glm::vec2((float)(_PlanetIndex + 1), 0.0f);
				_Planet->Scale = glm::vec2(1.0f, 1.0f) * 0.5f;
				_Planet->Angle = 0.0f;
				_Planet->RotationFrequency = 0.0f;

				_Sun->Childs.push_back(_Planet);

				size_t _SatelitesCount = Lehmer() % _MaxPlanetsCount;

				for (size_t _SateliteIndex = 0; _SateliteIndex < _SatelitesCount; _SateliteIndex++)
				{
					Entity* _Satelite = new Entity();

					_Satelite->Parent = _Planet;
					_Satelite->Position = glm::vec2((float)(_SateliteIndex + 1), 0.0f);
					_Satelite->Scale = glm::vec2(1.0f, 1.0f) * 0.25f;
					_Satelite->Angle = 0.0f;
					_Satelite->RotationFrequency = 0.0f;

					_Planet->Childs.push_back(_Satelite);
				}
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
