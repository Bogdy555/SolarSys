#ifndef SolarFuel_Dependencies_hpp

#define SolarFuel_Dependencies_hpp



#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glad.lib")



#include "SolarFuel_WinTargetVer.hpp"
#include <Windows.h>
#include <glad.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <random>
#include <cmath>
#include <vector>
#include <queue>



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

	namespace Scene
	{

		struct Entity;

	}

	namespace Graphics
	{

		struct Camera;

		struct Shader;

		struct Material;

		struct RenderObject;

		struct Renderer;

	}

}



#endif
