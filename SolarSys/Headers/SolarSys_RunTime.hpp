#ifndef SolarSys_RunTime_hpp

#define SolarSys_RunTime_hpp



#include "SolarSys.hpp"



namespace SolarSys
{

	namespace RunTime
	{

		class Application : public SolarFuel::RunTime::Application
		{

		public:

			Application();
			Application(const Application& _Other) = delete;
			Application(Application&& _Other) noexcept = delete;
			~Application();

			SolarFuel::Window& GetMainWindow();
			const SolarFuel::Window& GetMainWindow() const;

			Window::Data& GetMainWindowData();
			const Window::Data& GetMainWindowData() const;

			SolarFuel::Graphics::Renderer& GetDefaultRenderer();
			const SolarFuel::Graphics::Renderer& GetDefaultRenderer() const;

			enum ShaderTags : const size_t
			{
				_DefaultShader = 0,
				_Planet1Shader = 1,
				_Planet2Shader = 2,
				_Planet3Shader = 3,
				_Planet4Shader = 4,
				_Planet5Shader = 5,
				_Sun1Shader = 6,
				_Sun2Shader = 7
			};

			SolarFuel::Graphics::Shader& GetShader(const size_t _Index);
			const SolarFuel::Graphics::Shader& GetShader(const size_t _Index) const;

			enum MaterialTags : const size_t
			{
				_DefaultMaterial = 0,
				_Planet1Material = 1,
				_Planet2Material = 2,
				_Planet3Material = 3,
				_Planet4Material = 4,
				_Planet5Material = 5,
				_Sun1Material = 6,
				_Sun2Material = 7
			};

			SolarFuel::Graphics::Material& GetMaterial(const size_t _Index);
			const SolarFuel::Graphics::Material& GetMaterial(const size_t _Index) const;

			void UpdateFullScreen();

			void operator= (const Application& _Other) = delete;
			void operator= (Application&& _Other) noexcept = delete;

		private:

			SolarFuel::Window MainWindow;
			Window::Data MainWindowData;
			std::mutex PlacementMutex;
			std::mutex RectMutex;
			std::mutex FullScreenMutex;
			std::mutex CloseMutex;
			std::mutex InputMutex;
			std::mutex MinSizeMutex;
			std::mutex ContextMutex;

			SolarFuel::Graphics::Renderer DefaultRenderer;

			std::vector<SolarFuel::Graphics::Shader> Shaders;

			std::vector<SolarFuel::Graphics::Material> Materials;

			bool InitMainWindow();
			void CleanUpMainWindow();

			bool InitAssets();
			void CleanUpAssets();

			void Setup() override;
			void Update() override;
			void Stop() override;

		};

		enum MenuTypes : const uint64_t
		{
			_MainMenu = 1
		};

		class MainMenu : public SolarFuel::RunTime::Menu
		{

		public:

			MainMenu();
			MainMenu(const MainMenu& _Other) = delete;
			MainMenu(MainMenu&& _Other) noexcept = delete;
			~MainMenu();

			const uint64_t GetType() const override;

			void operator= (const MainMenu& _Other) = delete;
			void operator= (MainMenu&& _Other) noexcept = delete;

		private:

			bool Keys[2][256];

			glm::vec2 Position;
			float Zoom;

			SolarFuel::Scene::Entity Scene;

			float ElapsedTime;

			void ResetSpawn();

			void Setup() override;
			void Update() override;
			void Stop() override;

			void Input();
			void DeleteInput();
			void Engine();
			void FrameBuild();

		};

	}

}



#endif
