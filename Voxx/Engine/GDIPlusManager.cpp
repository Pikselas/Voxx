#include"GDIPlusManager.h"

unsigned int GDIPlusManager::ObjCount = 0;

GDIPlusManager::GDIPlusManager()
{
	if (!ObjCount++)
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&token, &input, nullptr);
	}
}

GDIPlusManager::~GDIPlusManager()
{
	if (!--ObjCount)
	{
		Gdiplus::GdiplusShutdown(token);
	}
}