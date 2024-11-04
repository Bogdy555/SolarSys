#ifndef SolarFuel_Scene_hpp

#define SolarFuel_Scene_hpp



#include "SolarFuel.hpp"



namespace SolarFuel
{

	namespace Scene
	{

		struct Entity
		{
			//hierarchy variables
			Entity* Parent = nullptr;
			std::vector<Entity*> Childs;

			//transform variables
			glm::vec2 Position;
			glm::vec2 Scale;
			float Angle;

			//rotation animation variable
			float RotationFrequency;

			//helper methods
			void DestroyAllChilds();
			glm::mat4 GetLocalMatrix();
			glm::mat4 GetWorldMatrix();
		};

	}

}



#endif
