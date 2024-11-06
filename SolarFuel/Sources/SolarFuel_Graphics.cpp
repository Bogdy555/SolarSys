#include "..\Headers\SolarFuel.hpp"



int (__stdcall *wglChoosePixelFormatARB)(HDC, const int*, const float*, unsigned int, int*, unsigned int*) = nullptr;
HGLRC (__stdcall *wglCreateContextAttribsARB)(HDC, HGLRC, const int*) = nullptr;
int (__stdcall *wglSwapIntervalEXT)(int) = nullptr;



bool SolarFuel::Graphics::Init()
{
	WNDCLASSEX _WndClass = { 0 };

	_WndClass.cbSize = sizeof(WNDCLASSEX);
	_WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	_WndClass.lpfnWndProc = DefWindowProc;
	_WndClass.cbClsExtra = 0;
	_WndClass.cbWndExtra = 0;
	_WndClass.hInstance = GetModuleHandle(nullptr);
	_WndClass.hIcon = NULL;
	_WndClass.hCursor = NULL;
	_WndClass.hbrBackground = NULL;
	_WndClass.lpszMenuName = nullptr;
	_WndClass.lpszClassName = L"DummyWndClass";
	_WndClass.hIconSm = NULL;

	if (!RegisterClassEx(&_WndClass))
	{
		return false;
	}

	HWND _hWnd = CreateWindowEx(NULL, _WndClass.lpszClassName, L"DummyWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, _WndClass.hInstance, nullptr);

	if (!_hWnd)
	{
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	HDC _WndDC = GetDC(_hWnd);

	if (!_WndDC)
	{
		DestroyWindow(_hWnd);
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	PIXELFORMATDESCRIPTOR _PFD = { 0 };

	_PFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	_PFD.nVersion = 1;
	_PFD.iPixelType = PFD_TYPE_RGBA;
	_PFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	_PFD.cColorBits = 32;
	_PFD.cAlphaBits = 8;
	_PFD.cDepthBits = 24;
	_PFD.cStencilBits = 8;
	_PFD.iLayerType = PFD_MAIN_PLANE;

	int _PForm = ChoosePixelFormat(_WndDC, &_PFD);

	if (!_PForm)
	{
		ReleaseDC(_hWnd, _WndDC);
		DestroyWindow(_hWnd);
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	if (!SetPixelFormat(_WndDC, _PForm, &_PFD))
	{
		ReleaseDC(_hWnd, _WndDC);
		DestroyWindow(_hWnd);
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	HGLRC wglContext = wglCreateContext(_WndDC);

	if (!wglContext)
	{
		ReleaseDC(_hWnd, _WndDC);
		DestroyWindow(_hWnd);
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	if (!wglMakeCurrent(_WndDC, wglContext))
	{
		wglDeleteContext(wglContext);
		ReleaseDC(_hWnd, _WndDC);
		DestroyWindow(_hWnd);
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	if (!gladLoadGL())
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(wglContext);
		ReleaseDC(_hWnd, _WndDC);
		DestroyWindow(_hWnd);
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	wglChoosePixelFormatARB = (int (__stdcall*)(HDC, const int*, const float*, unsigned int, int*, unsigned int*))(wglGetProcAddress("wglChoosePixelFormatARB"));
	wglCreateContextAttribsARB = (HGLRC (__stdcall*)(HDC, HGLRC, const int*))(wglGetProcAddress("wglCreateContextAttribsARB"));
	wglSwapIntervalEXT = (int (__stdcall*)(int))(wglGetProcAddress("wglSwapIntervalEXT"));

	if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB || !wglSwapIntervalEXT)
	{
		wglChoosePixelFormatARB = nullptr;
		wglCreateContextAttribsARB = nullptr;
		wglSwapIntervalEXT = nullptr;
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(wglContext);
		ReleaseDC(_hWnd, _WndDC);
		DestroyWindow(_hWnd);
		UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);
		return false;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(wglContext);
	ReleaseDC(_hWnd, _WndDC);
	DestroyWindow(_hWnd);
	UnregisterClass(_WndClass.lpszClassName, _WndClass.hInstance);

	return true;
}

void SolarFuel::Graphics::Stop()
{
	wglChoosePixelFormatARB = nullptr;
	wglCreateContextAttribsARB = nullptr;
	wglSwapIntervalEXT = nullptr;
}



const glm::mat4 SolarFuel::Graphics::Camera::GetViewMatrix() const
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(Angle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-Position.x, -Position.y, 0.0f));
}

const glm::mat4 SolarFuel::Graphics::Camera::GetProjectionMatrix(const float _AspectRatio) const
{
	return glm::ortho(-FieldOfView / 2.0f * _AspectRatio, FieldOfView / 2.0f * _AspectRatio, -FieldOfView / 2.0f, FieldOfView / 2.0f);
}



bool SolarFuel::Graphics::Shader::Load(const LPCWSTR _VertResId, const LPCWSTR _FragResId)
{
	Destroy();

	HINSTANCE _hInstance = GetModuleHandle(NULL);

	HRSRC _VertResHandle = FindResource(_hInstance, _VertResId, MAKEINTRESOURCE(SOLAR_FUEL_GLSL_RESOURCE));

	if (!_VertResHandle)
	{
		return false;
	}

	size_t _VertResSize = SizeofResource(_hInstance, _VertResHandle);

	if (!_VertResSize)
	{
		return false;
	}

	const char* _VertResMemory = (const char*)(LoadResource(_hInstance, _VertResHandle));

	if (!_VertResMemory)
	{
		return false;
	}

	char* _VertSrc = new char[_VertResSize + 1];

	if (!_VertSrc)
	{
		FreeResource((HGLOBAL)(_VertResMemory));
		return false;
	}

	_VertSrc[_VertResSize] = '\0';

	for (size_t _Index = 0; _Index < _VertResSize; _Index++)
	{
		_VertSrc[_Index] = _VertResMemory[_Index];
	}

	FreeResource((HGLOBAL)(_VertResMemory));

	HRSRC _FragResHandle = FindResource(_hInstance, _FragResId, MAKEINTRESOURCE(SOLAR_FUEL_GLSL_RESOURCE));

	if (!_FragResHandle)
	{
		delete[] _VertSrc;
		return false;
	}

	size_t _FragResSize = SizeofResource(_hInstance, _FragResHandle);

	if (!_FragResSize)
	{
		delete[] _VertSrc;
		return false;
	}

	const char* _FragResMemory = (const char*)(LoadResource(_hInstance, _FragResHandle));

	if (!_FragResMemory)
	{
		delete[] _VertSrc;
		return false;
	}

	char* _FragSrc = new char[_FragResSize + 1];

	if (!_FragSrc)
	{
		FreeResource((HGLOBAL)(_FragResMemory));
		delete[] _VertSrc;
		return false;
	}

	_FragSrc[_FragResSize] = '\0';

	for (size_t _Index = 0; _Index < _FragResSize; _Index++)
	{
		_FragSrc[_Index] = _FragResMemory[_Index];
	}

	FreeResource((HGLOBAL)(_FragResMemory));

	Id = glCreateProgram();

	if (!Id)
	{
		delete[] _VertSrc;
		delete[] _FragSrc;
		return false;
	}

	unsigned int _VertShader = glCreateShader(GL_VERTEX_SHADER);

	if (!_VertShader)
	{
		glDeleteProgram(Id);
		Id = 0;
		delete[] _VertSrc;
		delete[] _FragSrc;
		return false;
	}

	glShaderSource(_VertShader, 1, &_VertSrc, nullptr);
	glCompileShader(_VertShader);

	unsigned int _FragShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (!_FragShader)
	{
		glDeleteShader(_VertShader);
		glDeleteProgram(Id);
		Id = 0;
		delete[] _VertSrc;
		delete[] _FragSrc;
		return false;
	}

	glShaderSource(_FragShader, 1, &_FragSrc, nullptr);
	glCompileShader(_FragShader);

	glAttachShader(Id, _VertShader);
	glAttachShader(Id, _FragShader);
	glLinkProgram(Id);
	glValidateProgram(Id);

	glDetachShader(Id, _VertShader);
	glDeleteShader(_VertShader);
	glDetachShader(Id, _FragShader);
	glDeleteShader(_FragShader);
	delete[] _VertSrc;
	delete[] _FragSrc;

	MvpId = glGetUniformLocation(Id, "u_Mvp");
	ColorId = glGetUniformLocation(Id, "u_Color");
	ElapsedTimeId = glGetUniformLocation(Id, "u_ElapsedTime");

	return true;
}

void SolarFuel::Graphics::Shader::Destroy()
{
	if (Id != 0)
	{
		glDeleteProgram(Id);
		Id = 0;
	}

	MvpId = -1;
	ColorId = -1;
	ElapsedTimeId = -1;
}



void SolarFuel::Graphics::Renderer::Init()
{
	Destroy();

	std::vector<float> VBO =
	{
		-0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 1.0f, 0.0f
	};

	std::vector<unsigned int> IBO =
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &QuadVAO);
	glBindVertexArray(QuadVAO);

	glGenBuffers(1, &QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, VBO.size() * sizeof(float), VBO.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &QuadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBO.size() * sizeof(unsigned int), IBO.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void SolarFuel::Graphics::Renderer::Destroy()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (QuadVBO)
	{
		glDeleteBuffers(1, &QuadVBO);
		QuadVBO = 0;
	}
	if (QuadIBO)
	{
		glDeleteBuffers(1, &QuadIBO);
		QuadIBO = 0;
	}
	if (QuadVAO)
	{
		glDeleteVertexArrays(1, &QuadVAO);
		QuadVAO = 0;
	}
}

void SolarFuel::Graphics::Renderer::StartScene(const Camera& _ActiveCamera, const float _AspectRatio, const float _ElapsedTime)
{
	ProjectionView = _ActiveCamera.GetProjectionMatrix(_AspectRatio) * _ActiveCamera.GetViewMatrix();
	RenderObjects.clear();
	ElapsedTime = _ElapsedTime;
}

void SolarFuel::Graphics::Renderer::Submit(const RenderObject& _Object)
{
	RenderObjects.push_back(_Object);
}

void SolarFuel::Graphics::Renderer::Flush()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (const RenderObject& _Object : RenderObjects)
	{
		glm::mat4 _Mvp = ProjectionView * _Object.Model;

		glBindVertexArray(QuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadIBO);

		glUseProgram(_Object.Material->Shader->Id);
		glUniformMatrix4fv(_Object.Material->Shader->MvpId, 1, GL_FALSE, &_Mvp[0][0]);
		glUniform4fv(_Object.Material->Shader->ColorId, 1, &_Object.Material->Color[0]);
		glUniform1fv(_Object.Material->Shader->ElapsedTimeId, 1, &ElapsedTime);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	ProjectionView = glm::mat4(1.0f);
	RenderObjects.clear();
	ElapsedTime = 0.0f;
}
