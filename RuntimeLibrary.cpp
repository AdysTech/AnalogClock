#include "stdafx.h"
#include "EventHandler.h"
#include "RuntimeLibrary.h"

HINSTANCE hInstLib;
SetLayeredWindow SetLayeredWindowPtr;

bool InitLibrary()
{
	hInstLib=GetModuleHandle("user32.dll");
	if(hInstLib)
		return true;
	else
		return false;
}

bool TrnsparencySupported()
{
	if (InitLibrary())
	{
		SetLayeredWindowPtr=(SetLayeredWindow) GetProcAddress(hInstLib,"SetLayeredWindowAttributes");
		if (SetLayeredWindowPtr!=NULL)
			return true;
		else
			return false;
	}
	else
		return false;
}

