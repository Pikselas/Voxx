#pragma once
#include<Windows.h>
#include<sstream>
#include<bitset>
#include<functional>
#include<atomic>

class Window
{
	public:
		class Exception
		{
		private:
			const HRESULT code;
			std::string message;
			std::string file;
			unsigned int line;
		public:
			Exception(HRESULT hr);
			std::string GetReason() const;
			unsigned int GetLine() const;
			std::string GetFile() const;
			HRESULT GetErrorCode() const;
		};
	private:
		unsigned int height;
		unsigned int width;
	public:
		HWND window_handle;
	public:
		Window(Window * Parent, DWORD exStyle, const std::string& window_class ,const std::string& title, DWORD Style, int x, int y, int width, int height);
		~Window();
	public:
		unsigned int GetHeight() const;
		unsigned int GetWidth() const;
		void Redraw() const;
		void Update() const;
		void SetPosition(int x , int y) const;
		void ChangeTitle(const std::string& title) const;
		void SetFocus() const;
	public:
		using EventDispatcher = void (&)(const HWND);
		void ProcessEvents(EventDispatcher e = DispatchWindowEvents) const;
	public:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	public:
		static void DispatchWindowEvents(const HWND handle = nullptr);
		static void DispatchWindowEventsNonBlocking(const HWND handle = nullptr);
};

#define THROW_IF_FAILED(hrcall) if(HRESULT hrcode = hrcall ; FAILED(hrcode)) throw Window::Exception(hrcode)