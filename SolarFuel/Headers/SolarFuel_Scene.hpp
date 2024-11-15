#ifndef SolarFuel_Scene_hpp

#define SolarFuel_Scene_hpp



#include "SolarFuel.hpp"



namespace SolarFuel
{

	namespace Scene
	{

		struct Entity
		{
			Entity* Parent = nullptr;
			std::vector<Entity*> Childs;

			glm::vec2 Position = glm::vec2(0.0f, 0.0f);
			glm::vec2 Scale = glm::vec2(1.0f, 1.0f);
			float Angle = 0.0f;

			float RotationFrequency = 0.0f;

			size_t Material = 0;

			void GenerateSystem(const glm::vec2& _CameraPosition, const float _Width, const float _Height, const float _ElapsedTime, const size_t _BeginMaterialsSuns, const size_t _EndMaterialsSuns, const size_t _BeginMaterialsPlanets, const size_t _EndMaterialsPlanets, const size_t _BeginMaterialsSatelites, const size_t _EndMaterialsSatelites);
			void DestroyAllChilds();
			const glm::mat4 GetLocalMatrix() const;
			const glm::mat4 GetWorldMatrix() const;
		};

	}

}



#endif
