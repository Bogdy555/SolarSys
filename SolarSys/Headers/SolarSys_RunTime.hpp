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

			bool InitMainWindow();
			void CleanUpMainWindow();

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
