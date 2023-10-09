#include"Window.h"

Window::Window(Window * Parent , DWORD exStyle , const std::string& window_class , const std::string& title, DWORD Style , int x , int y , int width, int height)
	: 
height(height), width(width)
{
	Style |= WS_VISIBLE;

	RECT wr = {0};
	wr.right = width;
	wr.bottom = height;

	AdjustWindowRect(&wr, Style, FALSE);

	window_handle = CreateWindowEx(exStyle, window_class.c_str(), title.c_str(), Style, x, y, wr.right - wr.left, wr.bottom - wr.top, Parent == nullptr ? nullptr : Parent->window_handle, nullptr,
		GetModuleHandle(nullptr), nullptr);

	if (window_handle == nullptr)
	{
		throw Exception(GetLastError());
	}
}

void Window::Update() const
{
	UpdateWindow(window_handle);
}

void Window::SetPosition(int x , int y) const
{
	SetWindowPos(window_handle, nullptr, x , y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::Redraw() const
{
	RedrawWindow(window_handle, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

Window::~Window()
{
	DestroyWindow(window_handle);
}

unsigned int Window::GetHeight() const
{
	return height;
}

unsigned int Window::GetWidth() const
{
	return width;
}

void Window::ChangeTitle(const std::string& title) const
{
	SetWindowText(window_handle, title.c_str());
}

void Window::SetFocus() const
{
	SetForegroundWindow(window_handle);
}

void Window::ProcessEvents(EventDispatcher e) const
{
	e(window_handle);
}

void Window::DispatchWindowEvents(const HWND handle)
{
	MSG msg;
	if (GetMessage(&msg, handle, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::DispatchWindowEventsNonBlocking(const HWND handle)
{
	MSG msg;
	if (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

Window::Exception::Exception(HRESULT hr) : code(hr)
{
	char * pcharBuff = nullptr;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pcharBuff), 0, nullptr);
	message = pcharBuff;
	//file = s.file_name();
	//line = s.line();
	LocalFree(pcharBuff);
}

std::string Window::Exception::GetReason() const
{
	return message;
}

unsigned int Window::Exception::GetLine() const
{
	return line;
}

std::string Window::Exception::GetFile() const
{
	return file;
}

HRESULT Window::Exception::GetErrorCode() const
{
	return code;
}
