#ifndef SolarFuel_Dependencies_hpp

#define SolarFuel_Dependencies_hpp



#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "glad.lib")



#include "SolarFuel_WinTargetVer.hpp"
#include <Windows.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>



namespace SolarFuel
{

	enum States : const size_t
	{
		_Previous = 0,
		_Current = 1
	};

	namespace Time
	{

		class Timer;

	}

	namespace RunTime
	{

		class Application;

		enum MenuTypes : const uint64_t;

		class Menu;

	}

	class Window;

}



#endif
