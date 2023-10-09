#include "CustomWindow.h"

CustomWindow::WindowClass CustomWindow::WindowClass::wndcls;

CustomWindow::WindowClass::WindowClass()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = classNm;
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW; //Enables window to take double click events + redrawing of window
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = DefWindowProc;
	//wc.hIcon = static_cast<HICON>(LoadImage(hinst, MAKEINTRESOURCE(IDI_ICON1) , IMAGE_ICON,48 , 48 , 0 ));
	//wc.hIconSm = static_cast<HICON>(LoadImage(hinst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32 , 32 , 0));
	RegisterClassEx(&wc);
}

CustomWindow::WindowClass::~WindowClass()
{
	UnregisterClass(classNm, GetModuleHandle(nullptr));
}

constexpr const char* CustomWindow::WindowClass::GetName()
{
	return classNm;
}

CustomWindow::CustomWindow(CustomWindow* Parent, DWORD exStyle, const std::string& title, DWORD Style, int x, int y, int width, int height)
	:
	Window(Parent, exStyle, WindowClass::GetName(), title, Style, x, y, width, height)
{
	SetWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(StaticEventHandler));
}

LRESULT CustomWindow::StaticEventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	CustomWindow* const wnd_ptr = reinterpret_cast<CustomWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	return wnd_ptr->EventHandler(handle, msgcode, wparam, lparam);
}

LRESULT CustomWindow::EventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	switch (msgcode)
	{
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lparam);
			mouse.x = pt.x;
			mouse.y = pt.y;
		}
	}
	switch (msgcode)
	{
		case WM_MOUSEMOVE:
			if (mouse.OnMove)
			{
				mouse.OnMove(*this);
			}
			break;
		case WM_LBUTTONUP:
			mouse.LeftPressed = false;
			if (mouse.OnLeftRelease)
			{
				mouse.OnLeftRelease(*this);
			}
			break;
		case WM_LBUTTONDOWN:
			mouse.LeftPressed = true;
			if (mouse.OnLeftPress)
			{
				mouse.OnLeftPress(*this);
			}
			break;
		case WM_LBUTTONDBLCLK:
			if (mouse.OnLeftDoubleClick)
			{
				mouse.OnLeftDoubleClick(*this);
			}
			break;
		case WM_RBUTTONUP:
			mouse.RightPressed = false;
			if (mouse.OnRightRelease)
			{
				mouse.OnRightRelease(*this);
			}
			break;
		case WM_RBUTTONDOWN:
		mouse.RightPressed = true;
		if (mouse.OnRightPress)
		{
			mouse.OnRightPress(*this);
		}
		break;
		case WM_RBUTTONDBLCLK:
			if (mouse.OnRightDoubleClick)
			{
				mouse.OnRightDoubleClick(*this);
			}
			break;
		case WM_MOUSEWHEEL:
		{
			mouse.wheelDelta = GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA;
			if (mouse.OnWheel)
			{
				mouse.OnWheel(*this);
			}
		}
		break;
		case WM_KEYDOWN:
		{
			//if repeated then bit 30 of lparam will be true
			constexpr unsigned int Mask = 0b1 << 30;
			bool repeated = ((Mask & lparam) != 0);
			if (!repeated || keyboard.IsRepeatEnabled())
			{
				keyboard.KEY_STAT[static_cast<unsigned char>(wparam)] = true;
				if (keyboard.OnKeyPress)
				{
					keyboard.OnKeyPress({ *this , static_cast<unsigned char>(wparam)  , repeated });
				}
			}
		}
		break;
		case WM_KEYUP:
		{
			keyboard.KEY_STAT[static_cast<unsigned char>(wparam)] = false;
			if (keyboard.OnKeyRelease)
			{
				keyboard.OnKeyRelease({ *this , static_cast<unsigned char>(wparam)  , false });
			}
		}
		break;
		case WM_CHAR:
		{
			constexpr unsigned int Mask = 0b1 << 30;
			bool repeated = ((Mask & lparam) != 0);
			if (keyboard.OnCharInput)
			{
				keyboard.OnCharInput({ *this , static_cast<unsigned char>(wparam) , repeated });
			}
		}
		break;
		case WM_KILLFOCUS:
			mouse.Reset();
			keyboard.Reset();
			break;
		case WM_COMMAND:
		{
			/*auto component = reinterpret_cast<WindowComponent*>(GetWindowLongPtr(reinterpret_cast<HWND>(lparam), GWLP_USERDATA));
			if (component)
				component->PerformDefaultAction(wparam);
			break;*/
		}
	}
	return DefWindowProc(handle, msgcode, wparam, lparam);
}

int CustomWindow::Mouse::GetWheelDelta() const
{
	return wheelDelta;
}

bool CustomWindow::Mouse::IsLeftPressed() const
{
	return LeftPressed;
}

bool CustomWindow::Mouse::IsRightPressed() const
{
	return RightPressed;
}

int CustomWindow::Mouse::GetX() const
{
	return x;
}

int CustomWindow::Mouse::GetY() const
{
	return y;
}

std::pair<int, int> CustomWindow::Mouse::GetXY() const
{
	return { x , y };
}

void CustomWindow::Mouse::Reset()
{
	x = y = -1;
	LeftPressed = RightPressed = false;
}

bool CustomWindow::KeyBoard::IsKeyDown(unsigned char keycode) const
{
	return KEY_STAT[keycode];
}

bool CustomWindow::KeyBoard::IsRepeatEnabled() const
{
	return REPEAT_ENABLED;
}

void CustomWindow::KeyBoard::EnableKeyRepeat()
{
	REPEAT_ENABLED = true;
}

void CustomWindow::KeyBoard::DisableKeyRepeat()
{
	REPEAT_ENABLED = false;
}

void CustomWindow::KeyBoard::Reset()
{
	KEY_STAT.reset();
}

CustomWindow::KeyBoard::EventT::EventT(CustomWindow& wnd, unsigned char code, bool repeat) : window(wnd), KEY_CODE(code), IS_REPEATED(repeat)
{}