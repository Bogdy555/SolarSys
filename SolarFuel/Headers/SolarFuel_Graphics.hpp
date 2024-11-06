#ifndef SolarFuel_Renderer_hpp

#define SolarFuel_Renderer_hpp



#include "SolarFuel.hpp"



#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_ACCELERATION_ARB 0x2003



extern int (__stdcall *wglChoosePixelFormatARB)(HDC, const int*, const float*, unsigned int, int*, unsigned int*);
extern HGLRC (__stdcall *wglCreateContextAttribsARB)(HDC, HGLRC, const int*);
extern int (__stdcall *wglSwapIntervalEXT)(int);



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

			bool Load(const LPCWSTR _VertResId, const LPCWSTR _FragResId);
			void Destroy();
		};

		struct Material
		{
			const Shader* Shader = nullptr;
			glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		};

		struct RenderObject
		{
			glm::mat4 Model = glm::mat4(1.0f);
			const Material* Material = nullptr;
		};

		struct Renderer
		{
			glm::mat4 ProjectionView = glm::mat4(1.0f);
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
