// AnalogClock.cpp : Defines the entry point for the application.

#include "StdAfx.h"
#include "Globals.h"
#include "resource.h"
#include "Bitmap.h"
#include "Clock.h"
#include "Menu.h"
#include "EventHandler.h"
#include "RuntimeLibrary.h"
#include "RegistryFunctions.h"
#include "AnalogClock.h"

//Clock variables
POINT CENTRE;
int RADIUS=68;

HINSTANCE g_hInstance;		/*This application's HINSTANCE */
HRGN g_hRgn;				/*WindowRegion*/
HBITMAP g_hBitMap;			/*Image for Skin*/
HMENU g_hPopupMenu;			/*Handle to Menu*/
HBRUSH g_hBackGndBrush;
HANDLE g_hMutex;
/*
 *  Window procedure
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uiMsg) {
    HANDLE_MSG(hwnd, WM_CREATE,		OnCreate);    
    HANDLE_MSG(hwnd, WM_DESTROY,	OnDestroy);
    //HANDLE_MSG(hwnd, WM_CLOSE,		OnDestroy);
    HANDLE_MSG(hwnd, WM_PAINT,		OnPaint);	
	HANDLE_MSG(hwnd, WM_MOUSEMOVE,	OnMouseMove);
	HANDLE_MSG(hwnd, WM_LBUTTONDOWN,OnLButtonDown );
	HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK,OnLButtonDown );
	HANDLE_MSG(hwnd, WM_LBUTTONUP,	OnLButtonUp );
	HANDLE_MSG(hwnd, WM_RBUTTONDOWN,OnRButtonDown);
	HANDLE_MSG(hwnd, WM_RBUTTONUP,	OnRButtonUp);
	HANDLE_MSG(hwnd, WM_COMMAND,	OnContextMenu);
	HANDLE_MSG(hwnd, WM_SYSCOMMAND,	OnSysCommand);
	HANDLE_MSG(hwnd, WM_MOVE,		OnMove);
	HANDLE_MSG(hwnd, WM_ERASEBKGND,	OnEraseBkgnd);

	case WM_PRINTCLIENT:	OnPaintClient(hwnd, (HDC)wParam); return 0;			
	case WM_ENTERMENULOOP:
		RemoveMenu(GetSystemMenu(hwnd,false),SC_MOVE,MF_BYCOMMAND);
		return DefWindowProc(hwnd, uiMsg, wParam, lParam);
	case WM_EXITMENULOOP:
		InsertMenu(GetSystemMenu(hwnd,false),0,MF_BYPOSITION|MF_STRING,SC_MOVE,"Move");
		return DefWindowProc(hwnd, uiMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uiMsg, wParam, lParam);
}

BOOL
InitApp(void)
{
    WNDCLASS wc;

	LOGBRUSH brBackGround;

	g_hBitMap=(HBITMAP)LoadImage(NULL,"./skin/AnalogClock.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	
	//Load default Image if one is not Present
	if (g_hBitMap==NULL)
		g_hBitMap=(HBITMAP)LoadImage(g_hInstance,MAKEINTRESOURCE(IDB_MainSkin),IMAGE_BITMAP,0,0,LR_DEFAULTCOLOR);
	brBackGround.lbStyle = BS_PATTERN;
	brBackGround.lbHatch = (long) g_hBitMap;
	g_hBackGndBrush=CreateBrushIndirect(&brBackGround); 
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInstance;
    wc.hIcon = LoadIcon(g_hInstance,MAKEINTRESOURCE(IDI_MAIN));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = g_hBackGndBrush;
    wc.lpszMenuName = NULL;
    wc.lpszClassName ="AnalogClock";

    if (!RegisterClass(&wc)) return FALSE;

    return TRUE;
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev,
                   LPSTR lpCmdLine, int nShowCmd)
{
    MSG msg;
    HWND hwnd;

    g_hInstance = hinst;
	char sMutexName[]="Ady's Analog Clock";

	//quit if already a process is running, but highlight the previous instance*/	
	g_hMutex=CreateMutex(NULL, FALSE,sMutexName); 
	if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
		HWND hPrivWin;
		if(ReadSetting(NULL,"MainWindow",(BYTE &) hPrivWin,sizeof(hPrivWin))>0)
			FlashWindow(hPrivWin,false);
			SetForegroundWindow(hPrivWin);			 
		return 1;
	}
	
	AnalogClock NewClock;
	RECT rcRect;
	hwnd=NewClock.CreateClock(hinst);  

		//just create non volatile key
		SaveSetting(NULL,NULL,REG_SZ,(BYTE &)TEXT("Please visit http://adarsha.mv.googlepages.com for Latest Version"),sizeof(TEXT("Please visit http://adarsha.mv.googlepages.com for Latest Version")));

		//Save window handle in the eregistry which will be used by future instances of the program		
		SaveSetting(NULL,"MainWindow",REG_BINARY,(BYTE &)hwnd,sizeof(HWND),true);		

		//SetWindowPos(hwnd,HWND_NOTOPMOST,X ,Y,bitmap.bmWidth,bitmap.bmHeight,SWP_FRAMECHANGED);        
		ShowWindow(hwnd, nShowCmd);
		GetWindowRect(hwnd,&rcRect);

		//initialize Clock variables
		CENTRE.x=CENTRE.y=(rcRect.right-4-rcRect.left)  /2;
		//= (rcRect.bottom-1-rcRect.top) /2;
		RADIUS= int(0.395*(rcRect.right-rcRect.left));
		while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

  /*      CoUninitialize();
    }*/

    return 0;
}
AnalogClock::AnalogClock(void)
{
}

AnalogClock::~AnalogClock(void)
{
}
HWND AnalogClock ::CreateClock(HINSTANCE hInst)
{ 
	WNDCLASS wc;
	HWND hwnd;
	BITMAP bitmap;
	short X,Y; long wPos;

	m_hBitMap=(HBITMAP)LoadImage(NULL,"./skin/AnalogClock.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	
	//Load default Image if one is not Present
	if (m_hBitMap==NULL)
		m_hBitMap=(HBITMAP)LoadImage(hInst,MAKEINTRESOURCE(IDB_MainSkin),IMAGE_BITMAP,0,0,LR_DEFAULTCOLOR);
	g_hBitMap=m_hBitMap;
	LOGBRUSH brBackGround;
	brBackGround.lbStyle = BS_PATTERN;
	brBackGround.lbHatch = (long) g_hBitMap;
	HBRUSH hBackGndBrush=CreateBrushIndirect(&brBackGround);

    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInstance;
    wc.hIcon = LoadIcon(g_hInstance,MAKEINTRESOURCE(IDI_MAIN));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground =hBackGndBrush; //CreateSolidBrush(RGB(128,128,128));
    wc.lpszMenuName = NULL;
    wc.lpszClassName ="AnalogClock";

    if (!RegisterClass(&wc)) return NULL;
	GetObject(g_hBitMap, sizeof(BITMAP), &bitmap);

		
	ReadSetting(NULL,"LastWindowPos",(unsigned char &) wPos,sizeof(wPos));	
	X=LOWORD(wPos);
	Y=HIWORD(wPos);
	X= short(X<0?0:(X>GetSystemMetrics(SM_CXSCREEN)?GetSystemMetrics(SM_CXSCREEN)-bitmap.bmWidth:X));
	Y= short(Y<0?0:(Y>GetSystemMetrics(SM_CYSCREEN)?GetSystemMetrics(SM_CYSCREEN)-bitmap.bmHeight:Y));

	hwnd = CreateWindow(
		"AnalogClock",                  /* Class Name */
		"AnalogClock",                  /* Title */
		WS_POPUP | WS_SYSMENU,			/* Style */
		X,Y,							/* Position */
		bitmap.bmWidth,bitmap.bmHeight, /* Size */		
		NULL,                           /* Parent */
		NULL,                           /* No menu */
		hInst,                          /* Instance */
		0);                             /* No special parameters */

    return hwnd;
}