#include "..\Headers\SolarFuel.hpp"



void SolarFuel::Scene::Entity::DestroyAllChilds()
{
	for (Entity* _Child : Childs)
	{
		_Child->DestroyAllChilds();
		delete _Child;
	}

	Childs.clear();
}

glm::mat4 SolarFuel::Scene::Entity::GetLocalMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(Angle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(Scale.x, Scale.y, 1.0f));
}

glm::mat4 SolarFuel::Scene::Entity::GetWorldMatrix()
{
	if (!Parent)
	{
		return GetLocalMatrix();
	}

	return Parent->GetWorldMatrix() * GetLocalMatrix();
}
