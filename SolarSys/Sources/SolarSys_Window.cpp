#include "..\Headers\SolarSys.hpp"



SolarSys::Window::Data::Data() : PlacementMutex(nullptr), Placement({ 0 }), RectMutex(nullptr), Rect({ 0 }), FullScreenMutex(nullptr), FullScreen(false), CloseMutex(nullptr), Close(false), InputMutex(nullptr), Focus(false), Keys(), MinSizeMutex(nullptr), MinWidth(700), MinHeight(400)
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = false;
	}
}

SolarSys::Window::Data::Data(const Data& _Other) : PlacementMutex(_Other.PlacementMutex), Placement(_Other.Placement), RectMutex(_Other.RectMutex), Rect(_Other.Rect), FullScreenMutex(_Other.FullScreenMutex), FullScreen(_Other.FullScreen), CloseMutex(_Other.CloseMutex), Close(_Other.Close), InputMutex(_Other.InputMutex), Focus(_Other.Focus), Keys(), MinSizeMutex(_Other.MinSizeMutex), MinWidth(_Other.MinWidth), MinHeight(_Other.MinHeight)
{
	for (size_t _Index = 0; _Index < 256; _Index++)
	{
		Keys[_Index] = _Other.Keys[_Index];
	}
}

SolarSys::Window::Data::Data(Data&& _Other) noexcept : PlacementMutex(_Other.PlacementMutex), Placement(_Other.Placement), RectMutex(_Other.RectMutex), Rect(_Other.Rect), FullScreenMutex(_Other.FullScreenMutex), FullScreen(_Other.FullScreen), CloseMutex(_Other.CloseMutex), Close(_Other.Close), InputMutex(_Other.InputMutex), Focus(_Other.Focus), Keys(), MinSizeMutex(_Other.MinSizeMutex), MinWidth(_Other.MinWidth), MinHeight(_Other.MinHeight)
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

	return true;
}

void SolarSys::Window::CleanUpWindow(SolarFuel::Window* _Wnd)
{
	Data& _WindowData = *(Data*)(_Wnd->GetUserData());
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
