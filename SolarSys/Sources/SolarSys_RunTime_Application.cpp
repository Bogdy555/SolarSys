#include "..\Headers\SolarSys.hpp"



SolarSys::RunTime::Application::Application() : SolarFuel::RunTime::Application(), MainWindow(), MainWindowData(), PlacementMutex(), RectMutex(), FullScreenMutex(), CloseMutex(), InputMutex(), MinSizeMutex()
{

}

SolarSys::RunTime::Application::~Application()
{

}

SolarFuel::Window& SolarSys::RunTime::Application::GetMainWindow()
{
	return MainWindow;
}

const SolarFuel::Window& SolarSys::RunTime::Application::GetMainWindow() const
{
	return MainWindow;
}

SolarSys::Window::Data& SolarSys::RunTime::Application::GetMainWindowData()
{
	return MainWindowData;
}

const SolarSys::Window::Data& SolarSys::RunTime::Application::GetMainWindowData() const
{
	return MainWindowData;
}

void SolarSys::RunTime::Application::UpdateFullScreen()
{
	MainWindowData.FullScreenMutex->lock();

	MainWindowData.FullScreen = !MainWindowData.FullScreen;

	if (MainWindowData.FullScreen)
	{
		MainWindowData.FullScreenMutex->unlock();

		MainWindowData.PlacementMutex->lock();

		GetWindowPlacement(MainWindow, &MainWindowData.Placement);

		MainWindowData.PlacementMutex->unlock();

		MainWindowData.RectMutex->lock();

		GetWindowRect(MainWindow, &MainWindowData.Rect);

		MainWindowData.RectMutex->unlock();

		HMONITOR _hMonitor = MonitorFromWindow(MainWindow, MONITOR_DEFAULTTOPRIMARY);

		MONITORINFOEX _MonitorInfo = { 0 };

		_MonitorInfo.cbSize = sizeof(MONITORINFOEX);

		GetMonitorInfo(_hMonitor, &_MonitorInfo);

		SetWindowLongPtr(MainWindow, GWL_STYLE, WS_POPUP);

		SetWindowPos(MainWindow, HWND_TOP, _MonitorInfo.rcMonitor.left, _MonitorInfo.rcMonitor.top, _MonitorInfo.rcMonitor.right - _MonitorInfo.rcMonitor.left, _MonitorInfo.rcMonitor.bottom - _MonitorInfo.rcMonitor.top, SWP_ASYNCWINDOWPOS);

		MainWindow.Show(SW_SHOW);
	}
	else
	{
		MainWindowData.FullScreenMutex->unlock();

		SetWindowLongPtr(MainWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		MainWindowData.PlacementMutex->lock();

		if (MainWindowData.Placement.showCmd != SW_NORMAL)
		{
			SetWindowPos(MainWindow, HWND_TOP, MainWindowData.Placement.rcNormalPosition.left, MainWindowData.Placement.rcNormalPosition.top, MainWindowData.Placement.rcNormalPosition.right - MainWindowData.Placement.rcNormalPosition.left, MainWindowData.Placement.rcNormalPosition.bottom - MainWindowData.Placement.rcNormalPosition.top, SWP_ASYNCWINDOWPOS);

			MainWindow.Show(MainWindowData.Placement.showCmd);

			MainWindowData.PlacementMutex->unlock();
		}
		else
		{
			MainWindowData.PlacementMutex->unlock();

			MainWindowData.RectMutex->lock();

			SetWindowPos(MainWindow, HWND_TOP, MainWindowData.Rect.left, MainWindowData.Rect.top, MainWindowData.Rect.right - MainWindowData.Rect.left, MainWindowData.Rect.bottom - MainWindowData.Rect.top, SWP_ASYNCWINDOWPOS);

			MainWindow.Show(SW_SHOW);

			MainWindowData.RectMutex->unlock();
		}
	}
}

bool SolarSys::RunTime::Application::InitMainWindow()
{
	WNDCLASSEX _WndClass = { 0 };

	_WndClass.cbSize = sizeof(WNDCLASSEX);
	_WndClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	_WndClass.lpfnWndProc = Window::WindowProcedure;
	_WndClass.cbClsExtra = 0;
	_WndClass.cbWndExtra = 0;
	_WndClass.hInstance = GetInstanceHandle();
	_WndClass.hIcon = LoadIcon(GetInstanceHandle(), MAKEINTRESOURCE(SOLAR_SYS_IDI_MAIN_ICON));
	_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	_WndClass.hbrBackground = NULL;
	_WndClass.lpszMenuName = nullptr;
	_WndClass.lpszClassName = SOLAR_SYS_WND_CLASS_NAME;
	_WndClass.hIconSm = LoadIcon(GetInstanceHandle(), MAKEINTRESOURCE(SOLAR_SYS_IDI_MAIN_ICON));

	if (!RegisterClassEx(&_WndClass))
	{
		return false;
	}

	MainWindowData.PlacementMutex = &PlacementMutex;
	MainWindowData.RectMutex = &RectMutex;
	MainWindowData.FullScreenMutex = &FullScreenMutex;
	MainWindowData.CloseMutex = &CloseMutex;
	MainWindowData.MinSizeMutex = &MinSizeMutex;
	MainWindowData.InputMutex = &InputMutex;
	MainWindowData.ContextMutex = &ContextMutex;

	if (!MainWindow.Create(NULL, SOLAR_SYS_WND_CLASS_NAME, L"SolarSys", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, HWND_DESKTOP, NULL, GetInstanceHandle(), nullptr, NULL, Window::InitWindowThread, Window::CleanUpWindowThread, Window::InitWindow, Window::CleanUpWindow, &MainWindowData))
	{
		MainWindowData = Window::Data();
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	SetSync(MainWindow.GetRefreshRate());

	return true;
}

void SolarSys::RunTime::Application::CleanUpMainWindow()
{
	if (!MainWindow.CheckOn())
	{
		return;
	}

	MainWindow.Destroy();
	MainWindowData = Window::Data();
	UnregisterClass(SOLAR_SYS_WND_CLASS_NAME, GetInstanceHandle());
}

void SolarSys::RunTime::Application::Setup()
{
	if (GetArgC() != 1)
	{
		Close(-1);
		return;
	}

	if (!InitMainWindow())
	{
		Close(-1);
		return;
	}

	if (!MainWindow.Show(GetShowCmd()))
	{
		Close(-1);
		return;
	}

	SetCurrentMenu(_MainMenu);
	TurnOn();
}

void SolarSys::RunTime::Application::Update()
{
	switch (GetCurrentMenu())
	{
	case _MainMenu:
	{
		MainMenu _Menu;
		_Menu.Run(this);
		break;
	}
	default:
	{
		Close(-1);
		break;
	}
	}
}

void SolarSys::RunTime::Application::Stop()
{
	CleanUpMainWindow();
}
