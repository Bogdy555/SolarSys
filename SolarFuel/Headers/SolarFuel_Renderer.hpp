#ifndef SolarFuel_Renderer_hpp

#define SolarFuel_Renderer_hpp



#include "SolarFuel.hpp"



namespace SolarFuel
{

	namespace Graphics
	{

		struct Camera
		{
			glm::vec2 Position = glm::vec2(0.0f, 0.0f);
			float Angle = 0.0f;
			float FieldOfView = 30.0f;

			const glm::mat4 GetViewMatrix() const;
			const glm::mat4 GetProjectionMatrix(const float _AspectRatio) const;
		};

		struct RenderObject
		{
			glm::mat4 Model = glm::mat4(1.0f);
			GLuint Shader = 0;
		};

		struct Renderer
		{
			glm::mat4 Projection;
			glm::mat4 View;
			std::vector<const RenderObject> RenderObjects;
			GLuint QuadVAO = 0, QuadVBO = 0, QuadIBO = 0;

			bool Init();
			void Destroy();

			void StartScene(const Camera& _ActiveCamera, const float _AspectRatio);
			void Submit(const RenderObject& _Object);
			void Flush();
		};

	}

}



#endif
