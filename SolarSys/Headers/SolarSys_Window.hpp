#ifndef SolarSys_Window_hpp

#define SolarSys_Window_hpp



#include "SolarSys.hpp"



namespace SolarSys
{

	namespace Window
	{

		struct Data
		{
			std::mutex* PlacementMutex;
			WINDOWPLACEMENT Placement;

			std::mutex* RectMutex;
			RECT Rect;

			std::mutex* FullScreenMutex;
			bool FullScreen;

			std::mutex* CloseMutex;
			bool Close;

			std::mutex* InputMutex;
			bool Focus;
			bool Keys[256];

			std::mutex* MinSizeMutex;
			int32_t MinWidth;
			int32_t MinHeight;

			std::mutex* ContextMutex;
			HGLRC Context;

			Data();
			Data(const Data& _Other);
			Data(Data&& _Other) noexcept;
			~Data();

			void operator= (const Data& _Other);
			void operator= (Data&& _Other) noexcept;
		};

		bool InitWindowThread(void* _UserData);
		void CleanUpWindowThread(void* _UserData);
		bool InitWindow(SolarFuel::Window* _Wnd);
		void CleanUpWindow(SolarFuel::Window* _Wnd);

		LRESULT CALLBACK WindowProcedure(_In_ HWND _hWnd, _In_ UINT _Msg, _In_ WPARAM _wParam, _In_ LPARAM _lParam);

	}

}



#endif
