#include "StandardWindow.h"

int StandardWindow::WindowCount = 0;

void StandardWindow::MainLoop(const StandardWindow* const window , EventDispatcher e)
{
	MSG msg;
	if (window != nullptr)
	{
		while (!window->Closed)
		{
			e(window->window_handle);
		}
	}
	else
	{
		while (WindowCount > 0)
		{
			e(nullptr);
		}
	}
}

StandardWindow::StandardWindow(const std::string& Title, unsigned int Width, unsigned int Height , StandardWindow* Parent)
	:
CustomWindow(Parent , 0 , Title , WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT , Width , Height )
{
	++WindowCount;
}

StandardWindow::~StandardWindow()
{
	if (!Closed)
	{
		DestroyWindow(window_handle);
		Closed = true;
		--WindowCount;
	}
}

bool StandardWindow::IsOpen() const
{
	return !Closed;
}

unsigned int StandardWindow::GetWindowCount() const
{
	return WindowCount;
}

void StandardWindow::SetIcon(const std::string& IconPath , unsigned int size_x, unsigned int size_y)
{
	auto icon = (HICON)LoadImage(NULL, IconPath.c_str(), IMAGE_ICON, size_x, size_y, LR_LOADFROMFILE);
	SendMessage(window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
	SendMessage(window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
}

void StandardWindow::SetIcon(const unsigned int& IconID , unsigned int size_x, unsigned int size_y)
{
	auto icon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IconID), IMAGE_ICON, size_x, size_y, 0);
	SendMessage(window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
	SendMessage(window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
}

LRESULT StandardWindow::EventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	switch (msgcode)
	{
		case WM_CLOSE:
			Closed = true;
			--WindowCount;
		break;
	}
	return CustomWindow::EventHandler(handle, msgcode, wparam, lparam);
}
