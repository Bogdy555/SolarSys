#include "..\Headers\SolarSys.hpp"



int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPWSTR _CmdLine, _In_ int _ShowCmd)
{
	if (CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED) != S_OK)
	{
		MessageBox(NULL, L"An unexpected error occurred.", L"Error!", MB_OK | MB_ICONERROR);
		return -1;
	}

	if (!SolarFuel::Time::Init())
	{
		MessageBox(NULL, L"An unexpected error occurred.", L"Error!", MB_OK | MB_ICONERROR);
		CoUninitialize();
		return -1;
	}

	SolarSys::RunTime::Application _Application;

	int32_t _ReturnValue = _Application.Run(_hInstance, _CmdLine, _ShowCmd);

	if (_ReturnValue != 0)
	{
		MessageBox(NULL, L"An unexpected error occurred.", L"Error!", MB_OK | MB_ICONERROR);
	}

	SolarFuel::Time::Stop();
	CoUninitialize();

	return _ReturnValue;
}
