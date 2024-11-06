#include "..\Headers\SolarSys.hpp"



SolarSys::RunTime::MainMenu::MainMenu() : Keys(), Position(), Zoom(), Scene(), ElapsedTime(0.0f)
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

void SolarSys::RunTime::MainMenu::ResetSpawn()
{
	Position = glm::vec2(0.0f, 0.0f);
	Zoom = 30.0f;
	ElapsedTime = 0.0f;
}

void SolarSys::RunTime::MainMenu::Setup()
{
	ResetSpawn();

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

	float _Speed = Zoom;
	float _ZoomSpeed = 10.0f;
	float _ZoomMin = 5.0f;

	if (Keys[SolarFuel::_Current]['D'])
	{
		Position.x += GetTimeStep() * _Speed;
	}

	if (Keys[SolarFuel::_Current]['A'])
	{
		Position.x -= GetTimeStep() * _Speed;
	}

	if (Keys[SolarFuel::_Current]['W'])
	{
		Position.y += GetTimeStep() * _Speed;
	}

	if (Keys[SolarFuel::_Current]['S'])
	{
		Position.y -= GetTimeStep() * _Speed;
	}

	if (Keys[SolarFuel::_Current]['Q'])
	{
		Zoom += GetTimeStep() * _ZoomSpeed;
	}

	if (Keys[SolarFuel::_Current]['E'])
	{
		Zoom -= GetTimeStep() * _ZoomSpeed;

		if (Zoom < _ZoomMin)
		{
			Zoom = _ZoomMin;
		}
	}

	ElapsedTime += GetTimeStep();

	uint64_t _Width, _Height;
	_MainWindow.GetClientSize(_Width, _Height);
	float _AspectRatio = (float)(_Width) / (float)(_Height);

	Scene.DestroyAllChilds();
	Scene.GenerateSystem(Position, Zoom * _AspectRatio * 10.0f, Zoom * 10.0f, ElapsedTime);

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

	float _AspectRatio = (float)(_Width) / (float)(_Height);

	PAINTSTRUCT _PaintStruct = { 0 };
	HDC _hDC = BeginPaint(_MainWindow.GetHandle(), &_PaintStruct);

	_MainWindowData.ContextMutex->lock();
	wglMakeCurrent(_hDC, _MainWindowData.Context);
	_MainWindowData.ContextMutex->unlock();

	glViewport(0, 0, (int)(_Width), (int)(_Height));

	SolarFuel::Graphics::Camera _ActiveCamera;

	_ActiveCamera.Position = Position;
	_ActiveCamera.Angle = 0.0f;
	_ActiveCamera.FieldOfView = Zoom;

	_Renderer.StartScene(_ActiveCamera, _AspectRatio);

	struct QueueObject
	{
		const SolarFuel::Scene::Entity* Entity = nullptr;
		const glm::mat4 Transform = glm::mat4(1.0f);
	};

	std::queue<QueueObject> _Queue;

	_Queue.push(QueueObject(&Scene, Scene.GetLocalMatrix()));

	while (_Queue.size())
	{
		QueueObject& _CurrentObject = _Queue.front();

		for (const SolarFuel::Scene::Entity* _Child : _CurrentObject.Entity->Childs)
		{
			SolarFuel::Graphics::RenderObject _RenderObject;

			_RenderObject.Model = _CurrentObject.Transform * _Child->GetLocalMatrix();
			_RenderObject.Material = &_ApplicationObj.GetMaterial(Application::_DefaultMaterial);

			_Renderer.Submit(_RenderObject);

			_Queue.push(QueueObject(_Child, _RenderObject.Model));
		}

		_Queue.pop();
	}

	_Renderer.Flush();

	wglSwapIntervalEXT(1);
	SwapBuffers(_hDC);

	wglMakeCurrent(NULL, NULL);
	EndPaint(_MainWindow.GetHandle(), &_PaintStruct);
}
