#pragma once
#include"Window.h"

class CustomWindow : public Window
{
private:
	class WindowClass
	{
	private:
		constexpr static const char* classNm = "PikselasWindowClass";
		WindowClass();
		~WindowClass();
		static WindowClass wndcls;
	public:
		constexpr static const char* GetName();
	};
public:
	class Mouse
	{
		friend CustomWindow;
	private:
		int x = -1;
		int y = -1;
		int wheelDelta = 0;
	private:
		bool LeftPressed = false;
		bool RightPressed = false;
	public:
		bool IsLeftPressed() const;
		bool IsRightPressed() const;
		int  GetWheelDelta() const;
		int GetX() const;
		int GetY() const;
		std::pair<int, int> GetXY() const;
		void Reset();
	public:
		using EventHandlerType = std::function<void(CustomWindow&)>;
		EventHandlerType OnMove = nullptr;
		EventHandlerType OnWheel = nullptr;
		EventHandlerType OnLeftPress = nullptr;
		EventHandlerType OnRightPress = nullptr;
		EventHandlerType OnLeftRelease = nullptr;
		EventHandlerType OnRightRelease = nullptr;
		EventHandlerType OnLeftDoubleClick = nullptr;
		EventHandlerType OnRightDoubleClick = nullptr;
	};
	class KeyBoard
	{
		friend CustomWindow;
	private:
		std::bitset<256> KEY_STAT;
		bool REPEAT_ENABLED = false;
	public:
		bool IsKeyDown(unsigned char keycode) const;
		bool IsRepeatEnabled() const;
		void EnableKeyRepeat();
		void DisableKeyRepeat();
		void Reset();
	public:
		struct EventT
		{
			CustomWindow& window;
			unsigned char KEY_CODE;
			bool IS_REPEATED;
			EventT(CustomWindow& wnd, unsigned char code, bool repeat);
		};
	public:
		using EventHandlerType = std::function<void(EventT)>;
		EventHandlerType OnKeyPress = nullptr;
		EventHandlerType OnKeyRelease = nullptr;
		EventHandlerType OnCharInput = nullptr;
	};
public:
	Mouse mouse;
	KeyBoard keyboard;
public:
	CustomWindow(CustomWindow* Parent, DWORD exStyle, const std::string& title, DWORD Style, int x, int y, int width, int height);
protected:
	static LRESULT CALLBACK StaticEventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CALLBACK EventHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};