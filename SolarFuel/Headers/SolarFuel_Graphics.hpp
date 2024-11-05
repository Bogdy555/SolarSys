#ifndef SolarFuel_Renderer_hpp

#define SolarFuel_Renderer_hpp



#include "SolarFuel.hpp"



namespace SolarFuel
{

	namespace Graphics
	{

		bool Init();
		void Stop();

		struct Camera
		{
			glm::vec2 Position = glm::vec2(0.0f, 0.0f);
			float Angle = 0.0f;
			float FieldOfView = 30.0f;

			const glm::mat4 GetViewMatrix() const;
			const glm::mat4 GetProjectionMatrix(const float _AspectRatio) const;
		};

		struct Shader
		{
			GLuint Id = 0;
			GLint MvpId = -1;
			GLint ColorId = -1;
			GLint TextureId = -1;
			GLint TextureSizeId = -1;
			GLint TexturePositionId = -1;
		};

		struct Material
		{
			const Shader* Shader = nullptr;
			glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			int Texture = 0;
			glm::vec2 TextureSize = glm::vec2(1.0f, 1.0f);
			glm::vec2 TexturePosition = glm::vec2(0.0f, 0.0f);
		};

		struct RenderObject
		{
			glm::mat4 Model = glm::mat4(1.0f);
			const Material* Material = nullptr;
		};

		struct Renderer
		{
			glm::mat4 Projection = glm::mat4(1.0f);
			glm::mat4 View = glm::mat4(1.0f);
			std::vector<RenderObject> RenderObjects;
			GLuint QuadVAO = 0;
			GLuint QuadVBO = 0;
			GLuint QuadIBO = 0;

			void Init();
			void Destroy();

			void StartScene(const Camera& _ActiveCamera, const float _AspectRatio);
			void Submit(const RenderObject& _Object);
			void Flush();
		};

	}

}



#endif
