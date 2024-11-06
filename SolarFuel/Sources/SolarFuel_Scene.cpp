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

	constexpr float _GridUnit = 25.0f;

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
			_Sun->Scale = glm::vec2(1.0f, 1.0f) * 10.0f;
			_Sun->RotationFrequency = (float)(Lehmer() % 50 + 30);
			_Sun->Angle = _ElapsedTime * _Sun->RotationFrequency + (float)(Lehmer() % 360);

			Childs.push_back(_Sun);

			size_t _PlanetsCount = Lehmer() % 5;

			for (size_t _PlanetIndex = 0; _PlanetIndex < _PlanetsCount; _PlanetIndex++)
			{
				Entity* _Planet = new Entity();

				_Planet->Parent = _Sun;
				_Planet->Position = glm::rotate(glm::mat4(1.0f), glm::radians((float)(Lehmer() % 360)), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4((float)(_PlanetIndex) * 2.0f + 6.0f, 0.0f, 0.0f, 1.0f);
				_Planet->Scale = glm::vec2(1.0f, 1.0f) * 1.0f;
				_Planet->RotationFrequency = (float)(Lehmer() % 30 + 10);
				_Planet->Angle = _ElapsedTime * _Planet->RotationFrequency + (float)(Lehmer() % 360);

				_Sun->Childs.push_back(_Planet);

				size_t _SatelitesCount = Lehmer() % 2;

				for (size_t _SateliteIndex = 0; _SateliteIndex < _SatelitesCount; _SateliteIndex++)
				{
					Entity* _Satelite = new Entity();

					_Satelite->Parent = _Planet;
					_Satelite->Position = glm::rotate(glm::mat4(1.0f), glm::radians((float)(Lehmer() % 360)), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4((float)(_SateliteIndex) * 1.0f + 1.5f, 0.0f, 0.0f, 1.0f);
					_Satelite->Scale = glm::vec2(1.0f, 1.0f) * 0.5f;
					_Satelite->RotationFrequency = (float)(Lehmer() % 10 + 5);
					_Satelite->Angle = _ElapsedTime * _Satelite->RotationFrequency + (float)(Lehmer() % 360);

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
	float _Sin = sinf(glm::radians(Angle));
	float _Cos = cosf(glm::radians(Angle));

	glm::mat4 _Transform = glm::mat4(1.0f);

	_Transform[0][0] = Scale.x * _Cos; _Transform[1][0] = -Scale.y * _Sin; _Transform[2][0] = 0.0f; _Transform[3][0] = Position.x;
	_Transform[0][1] = Scale.x * _Sin; _Transform[1][1] = Scale.y * _Cos; _Transform[2][1] = 0.0f; _Transform[3][1] = Position.y;
	_Transform[0][2] = 0.0f; _Transform[1][2] = 0.0f; _Transform[2][2] = 0.0f; _Transform[3][2] = 0.0f;
	_Transform[0][3] = 0.0f; _Transform[1][3] = 0.0f; _Transform[2][3] = 0.0f; _Transform[3][3] = 1.0f;

	return _Transform;
}

const glm::mat4 SolarFuel::Scene::Entity::GetWorldMatrix() const
{
	if (!Parent)
	{
		return GetLocalMatrix();
	}

	return Parent->GetWorldMatrix() * GetLocalMatrix();
}
