#include "..\Headers\SolarFuel.hpp"



SolarFuel::RunTime::Application::Application() :
	On(false), ReturnValue(-1),
	CurrentMenu(_NullMenu),
	FrameTime(), LagTime(1.0f / 10.0f), SimulationSpeed(1.0f), Sync(60),
	InstanceHandle(NULL), CmdLine(nullptr), ShowCmd(SW_HIDE)
{

}

SolarFuel::RunTime::Application::~Application()
{

}

int32_t SolarFuel::RunTime::Application::Run(const HINSTANCE _InstanceHandle, const LPWSTR _CmdLine, const int32_t _ShowCmd)
{
	InstanceHandle = _InstanceHandle;
	CmdLine = _CmdLine;
	ShowCmd = _ShowCmd;

	Setup();
	while (On)
	{
		Update();
	}
	Stop();

	CurrentMenu = _NullMenu;

	FrameTime[_Previous].Start();
	FrameTime[_Current].Start();
	LagTime = 1.0f / 10.0f;
	SimulationSpeed = 1.0f;
	Sync = 60;

	InstanceHandle = NULL;
	CmdLine = nullptr;
	ShowCmd = SW_HIDE;

	int32_t _ReturnValue = ReturnValue;
	ReturnValue = -1;

	return _ReturnValue;
}

void SolarFuel::RunTime::Application::TurnOn()
{
	On = true;
}

void SolarFuel::RunTime::Application::Close(const int32_t _ReturnValue)
{
	On = false;
	ReturnValue = _ReturnValue;
}

void SolarFuel::RunTime::Application::SetCurrentMenu(const uint64_t _CurrentMenu)
{
	CurrentMenu = _CurrentMenu;
}

void SolarFuel::RunTime::Application::UpdateCurrentFrameTime()
{
	FrameTime[_Current].Stop();
}

void SolarFuel::RunTime::Application::SetLagTime(const float _LagTime)
{
	LagTime = _LagTime;
}

void SolarFuel::RunTime::Application::SetSimulationSpeed(const float _SimulationSpeed)
{
	SimulationSpeed = _SimulationSpeed;
}

void SolarFuel::RunTime::Application::SetSync(const uint64_t _Sync)
{
	Sync = _Sync;
}

const bool SolarFuel::RunTime::Application::CheckOn() const
{
	return On;
}

const int32_t SolarFuel::RunTime::Application::GetReturnValue() const
{
	return ReturnValue;
}

const uint64_t SolarFuel::RunTime::Application::GetCurrentMenu() const
{
	return CurrentMenu;
}

SolarFuel::Time::Timer& SolarFuel::RunTime::Application::GetFrameTime(const size_t _Index)
{
	return FrameTime[_Index];
}

const SolarFuel::Time::Timer& SolarFuel::RunTime::Application::GetFrameTime(const size_t _Index) const
{
	return FrameTime[_Index];
}

const float SolarFuel::RunTime::Application::GetTimeStep() const
{
	if (LagTime != 0.0f && FrameTime[_Previous] >= LagTime)
	{
		return LagTime * SimulationSpeed;
	}

	return FrameTime[_Previous] * SimulationSpeed;
}

const float SolarFuel::RunTime::Application::GetLagTime() const
{
	return LagTime;
}

const float SolarFuel::RunTime::Application::GetSimulationSpeed() const
{
	return SimulationSpeed;
}

const uint64_t SolarFuel::RunTime::Application::GetSync() const
{
	return Sync;
}

const HINSTANCE SolarFuel::RunTime::Application::GetInstanceHandle() const
{
	return InstanceHandle;
}

const LPWSTR SolarFuel::RunTime::Application::GetCmdLine() const
{
	return CmdLine;
}

const int32_t SolarFuel::RunTime::Application::GetShowCmd() const
{
	return ShowCmd;
}

const size_t SolarFuel::RunTime::Application::GetArgC() const
{
	return __argc;
}

const wchar_t* SolarFuel::RunTime::Application::GetArgV(const size_t _Index) const
{
	return __wargv[_Index];
}



SolarFuel::RunTime::Menu::Menu() : On(false), NextMenu(_NullMenu), ApplicationObj(nullptr)
{

}

SolarFuel::RunTime::Menu::~Menu()
{

}

void SolarFuel::RunTime::Menu::Run(Application* _ApplicationObj)
{
	if (!_ApplicationObj)
	{
		return;
	}

	ApplicationObj = _ApplicationObj;

	Setup();
	while (On)
	{
		GetFrameTime(_Current).Start();

		Update();

		if (GetSync())
		{
			GetFrameTime(_Current).Stop();

			if (GetFrameTime(_Current) < 1.0f / (float)(GetSync()))
			{
				std::this_thread::sleep_for(std::chrono::microseconds((uint64_t)(floorf((1.0f / (float)(GetSync()) - GetFrameTime(_Current)) * 1000000.0f))));
			}
		}

		GetFrameTime(_Current).Stop();

		GetFrameTime(_Previous) = GetFrameTime(_Current);

		if (!ApplicationObj->CheckOn())
		{
			Close(_NullMenu);
		}
	}
	Stop();

	ApplicationObj->SetCurrentMenu(NextMenu);
	NextMenu = _NullMenu;

	ApplicationObj = nullptr;
}

void SolarFuel::RunTime::Menu::TurnOn()
{
	On = true;
}

void SolarFuel::RunTime::Menu::Close(const uint64_t _NextMenu)
{
	On = false;
	NextMenu = _NextMenu;
}

void SolarFuel::RunTime::Menu::UpdateCurrentFrameTime()
{
	ApplicationObj->UpdateCurrentFrameTime();
}

void SolarFuel::RunTime::Menu::SetLagTime(const float _LagTime)
{
	ApplicationObj->SetLagTime(_LagTime);
}

void SolarFuel::RunTime::Menu::SetSimulationSpeed(const float _SimulationSpeed)
{
	ApplicationObj->SetSimulationSpeed(_SimulationSpeed);
}

void SolarFuel::RunTime::Menu::SetSync(const uint64_t _Sync)
{
	ApplicationObj->SetSync(_Sync);
}

const bool SolarFuel::RunTime::Menu::CheckOn() const
{
	return On;
}

const uint64_t SolarFuel::RunTime::Menu::GetNextMenu() const
{
	return NextMenu;
}

SolarFuel::RunTime::Application* SolarFuel::RunTime::Menu::GetApplicationObj()
{
	return ApplicationObj;
}

const SolarFuel::RunTime::Application* SolarFuel::RunTime::Menu::GetApplicationObj() const
{
	return ApplicationObj;
}

SolarFuel::Time::Timer& SolarFuel::RunTime::Menu::GetFrameTime(const size_t _Index)
{
	return ApplicationObj->GetFrameTime(_Index);
}

const SolarFuel::Time::Timer& SolarFuel::RunTime::Menu::GetFrameTime(const size_t _Index) const
{
	return ApplicationObj->GetFrameTime(_Index);
}

const float SolarFuel::RunTime::Menu::GetTimeStep() const
{
	return ApplicationObj->GetTimeStep();
}

const float SolarFuel::RunTime::Menu::GetLagTime() const
{
	return ApplicationObj->GetLagTime();
}

const float SolarFuel::RunTime::Menu::GetSimulationSpeed() const
{
	return ApplicationObj->GetSimulationSpeed();
}

const uint64_t SolarFuel::RunTime::Menu::GetSync() const
{
	return ApplicationObj->GetSync();
}
