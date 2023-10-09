#pragma once
#include<Windows.h>
#include<gdiplus.h>
#pragma comment( lib,"gdiplus.lib" )

class GDIPlusManager
{
private:
	ULONG_PTR token = 0;
	static unsigned int ObjCount;
public:
	GDIPlusManager();
	~GDIPlusManager();
};