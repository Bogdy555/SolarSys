#include "..\Headers\SolarSys.hpp"



SolarSys::RunTime::MainMenu::MainMenu() : Keys()
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[SolarFuel::_Previous][_Index] = false;
		Keys[SolarFuel::_Current][_Index] = false;
	}
}

SolarSys::RunTime::MainMenu::~MainMenu()
{

}

const uint64_t SolarSys::RunTime::MainMenu::GetType() const
{
	return _MainMenu;
}

void SolarSys::RunTime::MainMenu::Setup()
{
	DeleteInput();

	TurnOn();
}

void SolarSys::RunTime::MainMenu::Update()
{
	Input();
	Engine();
	FrameBuild();
}

void SolarSys::RunTime::MainMenu::Stop()
{

}

void SolarSys::RunTime::MainMenu::Input()
{
	Application& _ApplicationObj = *(Application*)(GetApplicationObj());

	SolarFuel::Window& _MainWindow = _ApplicationObj.GetMainWindow();

	Window::Data& _MainWindowData = _ApplicationObj.GetMainWindowData();

	_MainWindowData.InputMutex->lock();

	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[SolarFuel::_Previous][_Index] = Keys[SolarFuel::_Current][_Index];
		Keys[SolarFuel::_Current][_Index] = _MainWindowData.Keys[_Index];
	}

	_MainWindowData.InputMutex->unlock();

	_MainWindowData.InputMutex->lock();

	if (!_MainWindowData.Focus)
	{
		DeleteInput();
	}

	_MainWindowData.InputMutex->unlock();
}

void SolarSys::RunTime::MainMenu::DeleteInput()
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[SolarFuel::_Previous][_Index] = false;
		Keys[SolarFuel::_Current][_Index] = false;
	}
}

void SolarSys::RunTime::MainMenu::Engine()
{
	Application& _ApplicationObj = *(Application*)(GetApplicationObj());

	SolarFuel::Window& _MainWindow = _ApplicationObj.GetMainWindow();

	Window::Data& _MainWindowData = _ApplicationObj.GetMainWindowData();

	if (Keys[SolarFuel::_Previous][VK_F11] && !Keys[SolarFuel::_Current][VK_F11])
	{
		_ApplicationObj.UpdateFullScreen();
	}

	if (Keys[SolarFuel::_Previous][VK_ESCAPE] && Keys[SolarFuel::_Current][VK_ESCAPE])
	{
		_ApplicationObj.Close(0);
	}

	_ApplicationObj.SetSync(_MainWindow.GetRefreshRate());

	_MainWindowData.CloseMutex->lock();

	if (_MainWindowData.Close)
	{
		_ApplicationObj.Close(0);
	}

	_MainWindowData.CloseMutex->unlock();
}

void SolarSys::RunTime::MainMenu::FrameBuild()
{
	Application& _ApplicationObj = *(Application*)(GetApplicationObj());
	SolarFuel::Window& _MainWindow = _ApplicationObj.GetMainWindow();
	Window::Data& _MainWindowData = _ApplicationObj.GetMainWindowData();
	SolarFuel::Graphics::Renderer& _Renderer = _ApplicationObj.GetDefaultRenderer();

	uint64_t _Width, _Height;
	_MainWindow.GetClientSize(_Width, _Height);

	HDC _hDC = GetDC(_MainWindow.GetHandle());
	_MainWindowData.ContextMutex->lock();
	wglMakeCurrent(_hDC, _MainWindowData.Context);
	_MainWindowData.ContextMutex->unlock();
	ReleaseDC(_MainWindow.GetHandle(), _hDC);

	PAINTSTRUCT _PaintStruct = { 0 };
	_hDC = BeginPaint(_MainWindow.GetHandle(), &_PaintStruct);

	glViewport(0, 0, _Width, _Height);

	//_Renderer.StartScene();

	//_Renderer.Submit();

	//_Renderer.Flush();

	wglSwapIntervalEXT(1);
	SwapBuffers(_hDC);

	EndPaint(_MainWindow.GetHandle(), &_PaintStruct);
	wglMakeCurrent(NULL, NULL);
}
