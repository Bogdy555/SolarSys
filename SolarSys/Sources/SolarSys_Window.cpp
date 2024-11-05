#include "..\Headers\SolarSys.hpp"



SolarSys::Window::Data::Data() : PlacementMutex(nullptr), Placement({ 0 }), RectMutex(nullptr), Rect({ 0 }), FullScreenMutex(nullptr), FullScreen(false), CloseMutex(nullptr), Close(false), InputMutex(nullptr), Focus(false), Keys(), MinSizeMutex(nullptr), MinWidth(700), MinHeight(400), ContextMutex(nullptr), Context(NULL)
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = false;
	}
}

SolarSys::Window::Data::Data(const Data& _Other) : PlacementMutex(_Other.PlacementMutex), Placement(_Other.Placement), RectMutex(_Other.RectMutex), Rect(_Other.Rect), FullScreenMutex(_Other.FullScreenMutex), FullScreen(_Other.FullScreen), CloseMutex(_Other.CloseMutex), Close(_Other.Close), InputMutex(_Other.InputMutex), Focus(_Other.Focus), Keys(), MinSizeMutex(_Other.MinSizeMutex), MinWidth(_Other.MinWidth), MinHeight(_Other.MinHeight), ContextMutex(_Other.ContextMutex), Context(_Other.Context)
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
	}
}

SolarSys::Window::Data::Data(Data&& _Other) noexcept : PlacementMutex(_Other.PlacementMutex), Placement(_Other.Placement), RectMutex(_Other.RectMutex), Rect(_Other.Rect), FullScreenMutex(_Other.FullScreenMutex), FullScreen(_Other.FullScreen), CloseMutex(_Other.CloseMutex), Close(_Other.Close), InputMutex(_Other.InputMutex), Focus(_Other.Focus), Keys(), MinSizeMutex(_Other.MinSizeMutex), MinWidth(_Other.MinWidth), MinHeight(_Other.MinHeight), ContextMutex(_Other.ContextMutex), Context(_Other.Context)
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
		_Other.Keys[_Index] = false;
	}

	_Other.PlacementMutex = nullptr;
	_Other.Placement = { 0 };
	_Other.RectMutex = nullptr;
	_Other.Rect = { 0 };
	_Other.FullScreenMutex = nullptr;
	_Other.FullScreen = false;
	_Other.CloseMutex = nullptr;
	_Other.Close = false;
	_Other.InputMutex = nullptr;
	_Other.Focus = false;
	_Other.MinSizeMutex = nullptr;
	_Other.MinWidth = 700;
	_Other.MinHeight = 400;
	_Other.ContextMutex = nullptr;
	_Other.Context = NULL;
}

SolarSys::Window::Data::~Data()
{

}

void SolarSys::Window::Data::operator= (const Data& _Other)
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
	}

	PlacementMutex = _Other.PlacementMutex;
	Placement = _Other.Placement;
	RectMutex = _Other.RectMutex;
	Rect = _Other.Rect;
	FullScreenMutex = _Other.FullScreenMutex;
	FullScreen = _Other.FullScreen;
	CloseMutex = _Other.CloseMutex;
	Close = _Other.Close;
	InputMutex = _Other.InputMutex;
	Focus = _Other.Focus;
	MinSizeMutex = _Other.MinSizeMutex;
	MinWidth = _Other.MinWidth;
	MinHeight = _Other.MinHeight;
	ContextMutex = _Other.ContextMutex;
	Context = _Other.Context;
}

void SolarSys::Window::Data::operator= (Data&& _Other) noexcept
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
		_Other.Keys[_Index] = false;
	}

	PlacementMutex = _Other.PlacementMutex;
	Placement = _Other.Placement;
	RectMutex = _Other.RectMutex;
	Rect = _Other.Rect;
	FullScreenMutex = _Other.FullScreenMutex;
	FullScreen = _Other.FullScreen;
	CloseMutex = _Other.CloseMutex;
	Close = _Other.Close;
	InputMutex = _Other.InputMutex;
	Focus = _Other.Focus;
	MinSizeMutex = _Other.MinSizeMutex;
	MinWidth = _Other.MinWidth;
	MinHeight = _Other.MinHeight;
	ContextMutex = _Other.ContextMutex;
	Context = _Other.Context;

	_Other.PlacementMutex = nullptr;
	_Other.Placement = { 0 };
	_Other.RectMutex = nullptr;
	_Other.Rect = { 0 };
	_Other.FullScreenMutex = nullptr;
	_Other.FullScreen = false;
	_Other.CloseMutex = nullptr;
	_Other.Close = false;
	_Other.InputMutex = nullptr;
	_Other.Focus = false;
	_Other.MinSizeMutex = nullptr;
	_Other.MinWidth = 700;
	_Other.MinHeight = 400;
	_Other.ContextMutex = nullptr;
	_Other.Context = NULL;
}



bool SolarSys::Window::InitWindowThread(void* _UserData)
{
	return true;
}

void SolarSys::Window::CleanUpWindowThread(void* _UserData)
{

}

bool SolarSys::Window::InitWindow(SolarFuel::Window* _Wnd)
{
	Data& _WindowData = *(Data*)(_Wnd->GetUserData());
	
	HWND _hWnd = _Wnd->GetHandle();

	HDC _WndDC = GetDC(_hWnd);

	if (!_WndDC)
	{
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
		return false;
	}

	if (!SetPixelFormat(_WndDC, _PForm, &_PFD))
	{
		ReleaseDC(_hWnd, _WndDC);
		return false;
	}

	HGLRC wglContext = wglCreateContext(_WndDC);

	if (!wglContext)
	{
		ReleaseDC(_hWnd, _WndDC);
		return false;
	}

	if (!wglMakeCurrent(_WndDC, wglContext))
	{
		wglDeleteContext(wglContext);
		ReleaseDC(_hWnd, _WndDC);
		return false;
	}

	return true;
}

void SolarSys::Window::CleanUpWindow(SolarFuel::Window* _Wnd)
{
	Data& _WindowData = *(Data*)(_Wnd->GetUserData());

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(_WindowData.Context);
}

LRESULT CALLBACK SolarSys::Window::WindowProcedure(_In_ HWND _hWnd, _In_ UINT _Msg, _In_ WPARAM _wParam, _In_ LPARAM _lParam)
{
	SolarFuel::Window* _Wnd = SolarFuel::Window::GetWindowPtr(_hWnd);

	Data* _WndData = (Data*)(_Wnd->GetUserData());

	switch (_Msg)
	{
	case WM_CLOSE:
	{
		_WndData->CloseMutex->lock();

		_WndData->Close = true;

		_WndData->CloseMutex->unlock();

		break;
	}
	case WM_KEYDOWN:
	{
		_WndData->InputMutex->lock();

		_WndData->Keys[_wParam] = true;

		_WndData->InputMutex->unlock();

		break;
	}
	case WM_KEYUP:
	{
		_WndData->InputMutex->lock();

		_WndData->Keys[_wParam] = false;

		_WndData->InputMutex->unlock();

		break;
	}
	case WM_SETFOCUS:
	{
		_WndData->InputMutex->lock();

		_WndData->Focus = true;

		for (size_t _Index = 0; _Index < 256; _Index++)
		{
			_WndData->Keys[_Index] = false;
		}

		_WndData->InputMutex->unlock();

		break;
	}
	case WM_KILLFOCUS:
	{
		_WndData->InputMutex->lock();

		_WndData->Focus = false;

		for (size_t _Index = 0; _Index < 256; _Index++)
		{
			_WndData->Keys[_Index] = false;
		}

		_WndData->InputMutex->unlock();

		break;
	}
	case WM_ERASEBKGND:
	{
		break;
	}
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO _MinMaxInfo = (LPMINMAXINFO)(_lParam);

		_WndData->MinSizeMutex->lock();

		_MinMaxInfo->ptMinTrackSize.x = _WndData->MinWidth;
		_MinMaxInfo->ptMinTrackSize.y = _WndData->MinHeight;

		_WndData->MinSizeMutex->unlock();

		break;
	}
	default:
	{
		return DefWindowProc(_hWnd, _Msg, _wParam, _lParam);
	}
	}

	return 0;
}
