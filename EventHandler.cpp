#include "StdAfx.h"
#include "Globals.h"
#include "resource.h"
#include "Bitmap.h"
#include "Clock.h"
#include "Menu.h"
#include "RuntimeLibrary.h"
#include "OptionDialogs.h"
#include "RegistryFunctions.h"
#include "Calendar.h"

//Global variables used to hold data
bool bTransBack=false;
bool bShowSeconds=true;
bool bSmothMovement=false;
bool bTopMost;
BYTE iTranslucency=255;


/*
 *  OnCreate
 *      Applications will typically override this and maybe even
 *      create a child window.
 */
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpcs)
{
	HMENU hSysMenu,hMenu;
	
	g_hPopupMenu=LoadMenu(g_hInstance,MAKEINTRESOURCE(IDR_SysMenu));	
	//clear up SystemMenu
	hSysMenu=GetSystemMenu(hwnd,false);
	//if i delete SC_MOVE windows will not allow me to move
//	RemoveMenu(hSysMenu,SC_MOVE,MF_BYCOMMAND);

	SetMenuState(hSysMenu,SC_MOVE,MFS_DISABLED);

	DeleteMenu(hSysMenu,SC_SIZE,MF_BYCOMMAND);
	DeleteMenu(hSysMenu,SC_MAXIMIZE,MF_BYCOMMAND);
	DeleteMenu(hSysMenu,SC_MINIMIZE,MF_BYCOMMAND);
	DeleteMenu(hSysMenu,SC_RESTORE,MF_BYCOMMAND);
	//DeleteMenu(hSysMenu,0,MF_BYPOSITION);
	DeleteMenu(hSysMenu,0,MF_BYCOMMAND);
	//DeleteMenu(hSysMenu,SC_CLOSE,MF_BYCOMMAND);

	hMenu=GetSubMenu(g_hPopupMenu, 0); 
	hMenu=GetSubMenu(hMenu,1); 
	
	InsertMenu(hSysMenu,SC_CLOSE,MF_BYCOMMAND|MF_POPUP,(UINT)hMenu,"&Transparency");
	InsertMenu(hSysMenu,SC_CLOSE,MF_BYCOMMAND|MF_POPUP,IDM_TopMost,"&StayOnTop");
	InsertMenu(hSysMenu,SC_CLOSE,MF_BYCOMMAND|MF_POPUP,IDM_About,"&About");
	InsertMenu(hSysMenu,SC_CLOSE,MF_BYCOMMAND|MF_SEPARATOR,0,"");

	DrawMenuBar(hwnd);




	ReadSetting(NULL,"Translucency",(unsigned char &) iTranslucency,sizeof(iTranslucency));
	
	if (TrnsparencySupported())
	{
	
		SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		//Set the Window Skin
		SetLayeredWindowAttributes(hwnd,GetColorAtPixel(g_hBitMap),iTranslucency,LWA_COLORKEY|LWA_ALPHA);

		//SetLayeredWindowAttributes(hwnd,NULL,iTranslucency,LWA_ALPHA);
		SetMenuState(g_hPopupMenu,IDM_TransFull,MFT_RADIOCHECK,true);
		SetMenuState(g_hPopupMenu,IDM_TransOpaque,MFT_RADIOCHECK,true);
		SetMenuState(g_hPopupMenu,IDM_TransCustom,MFT_RADIOCHECK,true);
		if (iTranslucency==255)
			SetMenuState(g_hPopupMenu,IDM_TransOpaque,MFS_CHECKED);
		else if(iTranslucency==10)
			SetMenuState(g_hPopupMenu,IDM_TransFull,MFS_CHECKED);
		else
			SetMenuState(g_hPopupMenu,IDM_TransCustom,MFS_CHECKED);


	}	
	else
	{
		//If the SetLayeredWindowAttributes is not availeble thne use traditional method of
		//joining pixel wide regions to match the Bitmap image.
		g_hRgn=CreateRegionFromBitmap(g_hBitMap);
		SetWindowRgn(hwnd,g_hRgn,TRUE); 		

		DeleteMenu(g_hPopupMenu,IDM_TransFull,MF_BYCOMMAND);
		DeleteMenu(g_hPopupMenu,IDM_TransOpaque,MF_BYCOMMAND);
		DeleteMenu(g_hPopupMenu,IDM_TransCustom,MF_BYCOMMAND);		
	}	
	
	//Get saved settings
	ReadSetting(NULL,"ShowSeconds",(unsigned char &) bShowSeconds,sizeof(bShowSeconds));
	if(!bShowSeconds) 
	{
		bSmothMovement=false;
		SetMenuState(g_hPopupMenu,IDM_ShowSeconds,MFS_UNCHECKED);
		SetMenuState(g_hPopupMenu,IDM_SmothMovement,MFS_DISABLED);
	}

	ReadSetting(NULL,"SmothMovement",(unsigned char &) bSmothMovement,sizeof(bSmothMovement));
	if(bSmothMovement) 			
		SetMenuState(g_hPopupMenu,IDM_SmothMovement,MFS_CHECKED);

	ReadSetting(NULL,"TransparentBackGnd",(unsigned char &) bTransBack,sizeof(bTransBack));
	if (bTransBack) 
		SetMenuState(g_hPopupMenu,IDM_TransBackGnd,MFS_CHECKED);
	
	ReadSetting(NULL,"TopMost",(unsigned char &) bTopMost,sizeof(bTopMost));
	if (bTopMost) 
		PostMessage(hwnd,WM_COMMAND,MAKEWPARAM(IDM_TopMost,0L),0L);

	//Get color Info
	if(ReadSetting("Colors","HourNumbers",(unsigned char &) clrHourNumber,sizeof(clrHourNumber))==-1)
		clrHourNumber=16711937;
	if(ReadSetting("Colors","MinuteTicks",(unsigned char &) clrMinuteTicks,sizeof(clrMinuteTicks))==-1)
		clrMinuteTicks=16777098;
	if(ReadSetting("Colors","WeekName",(unsigned char &) clrWeek,sizeof(clrWeek))==-1)
		clrWeek=8092417;
	if(ReadSetting("Colors","Date",(unsigned char &) clrDate,sizeof(clrDate))==-1)
		clrDate=6160895;
	if(ReadSetting("Colors","SecondHand",(unsigned char &) clrSecondHand,sizeof(clrSecondHand))==-1)
		clrSecondHand=9437695;
	if(ReadSetting("Colors","MinuteHand",(unsigned char &) clrMinuteHand,sizeof(clrMinuteHand))==-1)
		clrMinuteHand=13203490;
	if(ReadSetting("Colors","HourHand",(unsigned char &) clrHourHand,sizeof(clrHourHand))==-1)
		clrHourHand=16764391;
	if(ReadSetting("Colors","CalendarBackGnd",(unsigned char &) clrCalendarBackGnd,sizeof(clrCalendarBackGnd))==-1)
		clrCalendarBackGnd=543622;
	if(ReadSetting("Colors","CalendarDate",(unsigned char &) clrCalendarDate ,sizeof(clrCalendarDate))==-1)
		clrCalendarDate=10811810;
    return 1;
}

/*
 *  OnDestroy
 *      Post a quit message because our application is over when the
 *      user closes this window.
 */
void OnDestroy(HWND hwnd)
{
	//save color Info
	SaveSetting("Colors","HourNumbers",REG_DWORD,(unsigned char &) clrHourNumber,sizeof(clrHourNumber));
	SaveSetting("Colors","MinuteTicks",REG_DWORD,(unsigned char &) clrMinuteTicks,sizeof(clrMinuteTicks));
	SaveSetting("Colors","WeekName",REG_DWORD,(unsigned char &) clrWeek,sizeof(clrWeek));
	SaveSetting("Colors","Date",REG_DWORD,(unsigned char &) clrDate,sizeof(clrDate));
	SaveSetting("Colors","SecondHand",REG_DWORD,(unsigned char &) clrSecondHand,sizeof(clrSecondHand));
	SaveSetting("Colors","MinuteHand",REG_DWORD,(unsigned char &) clrMinuteHand,sizeof(clrMinuteHand));
	SaveSetting("Colors","HourHand",REG_DWORD,(unsigned char &) clrHourHand,sizeof(clrHourHand));
	SaveSetting("Colors","CalendarBackGnd",REG_DWORD,(unsigned char &) clrCalendarBackGnd,sizeof(clrCalendarBackGnd));
	SaveSetting("Colors","CalendarDate",REG_DWORD,(unsigned char &) clrCalendarDate ,sizeof(clrCalendarDate));


	//save settings
	SaveSetting(NULL,"ShowSeconds",REG_BINARY,(unsigned char &) bShowSeconds,sizeof(bShowSeconds));
	SaveSetting(NULL,"SmothMovement",REG_BINARY,(unsigned char &) bSmothMovement,sizeof(bSmothMovement));
	SaveSetting(NULL,"TransparentBackGnd",REG_BINARY,(unsigned char &) bTransBack,sizeof(bTransBack));
	SaveSetting(NULL,"Translucency",REG_BINARY,(unsigned char &) iTranslucency,sizeof(iTranslucency));	
	SaveSetting(NULL,"TopMost",REG_BINARY,(unsigned char &) bTopMost,sizeof(bTopMost));	

	RECT rt;
	GetWindowRect(hwnd,&rt);
	DWORD wPos=MAKELONG(rt.left,rt.top);  
	SaveSetting(NULL,"LastWindowPos",REG_BINARY,(unsigned char &) wPos,sizeof(wPos));


	DeleteObject(g_hBitMap);
	DeleteObject(g_hRgn);
	DestroyMenu(g_hPopupMenu); 
	CloseHandle(g_hMutex);
	RemoveSetting(NULL,"MainWindow");
    PostQuitMessage(0);
}



/*
 *  PaintContent
 *      Interesting things will be painted here eventually.
 */
void PaintContent(HWND hwnd, PAINTSTRUCT *pps)
{
	/*if(bTransBack)
	{
		MakeWindowTransparent(pps->hdc,hwnd,g_hRgn);	
		PostMessage(hwnd,WM_ERASEBKGND,(WPARAM)pps->hdc,0L);
		InvalidateRgn(hwnd, g_hRgn, TRUE);
	}*/
	WaitForSingleObject(g_hMutex,100); 
	DrawClockFace(pps->hdc);
	DrawClockHands(pps->hdc,bShowSeconds,bSmothMovement);
	ReleaseMutex(g_hMutex); 
}

//Periodic updates to window
VOID CALLBACK InvalidateAndKillTimer(HWND hwnd,UINT uMsg,UINT idTimer,DWORD dwTime)
{
    KillTimer(hwnd, idTimer);
	InvalidateRect(hwnd, NULL, TRUE);

}


/*
 *  OnPaint
 *      Paint the content as part of the paint cycle.
 */
void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
	RECT rcClip;

	/*Paint Only if atleast a part of the window is visible*/
	if (GetClipBox(ps.hdc, &rcClip)!=NULLREGION) 
	{
		// compute time to next update - we update once a second
		SYSTEMTIME st;DWORD dwTimeToNextTick;
		PaintContent(hwnd, &ps);
        GetLocalTime(&st);
		if (!bSmothMovement)
			dwTimeToNextTick = 1000 - st.wMilliseconds;
		else
			dwTimeToNextTick =  167;
        SetTimer(hwnd, 1, dwTimeToNextTick, InvalidateAndKillTimer);
	}
	else
        SetTimer(hwnd, 1, 5000, InvalidateAndKillTimer);

	EndPaint(hwnd, &ps);
}

/*
 *  OnPaintClient
 *      Paint the content as requested by USER.
 */
void OnPaintClient(HWND hwnd, HDC hdc)
{
    PAINTSTRUCT ps;
    ps.hdc = hdc;
    GetClientRect(hwnd, &ps.rcPaint);
    PaintContent(hwnd, &ps);

}

void OnLButtonDown( HWND hwnd, BOOL fDblClk, int x, int y, UINT flags )
{
	if (fDblClk)
		ShowCalendar(hwnd);
}

void OnLButtonUp( HWND hwnd, int x, int y, UINT flags )
{

}

void OnRButtonDown( HWND hwnd, BOOL fDblClk, int x, int y, UINT flags )
{

	PostMessage(hwnd,WM_NCRBUTTONDOWN,HTCAPTION,MAKELPARAM(10,10));

}
void OnRButtonUp( HWND hwnd, int x, int y, UINT flags )
{
	HMENU hMenu; POINT MenuCoord;
	hMenu=GetSubMenu(g_hPopupMenu, 0); 
	//x,y are relative to 0,0 of the window, but we need relative to 0,0 of the screen to display the menu
	MenuCoord.x=x;
	MenuCoord.y=y;
	ClientToScreen(hwnd,&MenuCoord);

	TrackPopupMenu(hMenu,TPM_LEFTALIGN | TPM_LEFTBUTTON,MenuCoord.x,MenuCoord.y,0,hwnd,NULL);
}

void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	
	/*For Dragging a borderless Form: Check for WM_LBUTTONDOWN, then change the message 
	and wparam to make windows believe that the user is clicking on the caption.
	(even though you don't have one). Set the message to WM_NCLBUTTONDOWN and the 
	wparam to HTCAPTION.*/
	if (keyFlags|MK_LBUTTON)
		PostMessage(hwnd,WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(x,y));


}


void OnContextMenu(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	RECT rt;bool iChoice;
	switch(id) 
	{
		case IDM_Exit:
			//PostMessage(hwnd,WM_DESTROY,NULL,NULL);
			//PostMessage(hwnd,WM_CLOSE,NULL,NULL);
			DestroyWindow(hwnd);
		    PostQuitMessage(0);
			break;
		case IDM_TopMost:
			GetWindowRect(hwnd,&rt);
			if(ToggleMenuCheckMark(g_hPopupMenu,id)) 
			{
				SetWindowPos(hwnd,HWND_NOTOPMOST,rt.left ,rt.top ,rt.right-rt.left+1 ,rt.bottom-rt.top+1,SWP_NOMOVE);
				bTopMost=false;
			}
			else
			{
				SetWindowPos(hwnd,HWND_TOPMOST,rt.left ,rt.top ,rt.right-rt.left+1 ,rt.bottom-rt.top+1,SWP_NOMOVE);
				bTopMost=true;
			}
			ToggleMenuCheckMark(GetSystemMenu(hwnd,false),id);
			break;
		case IDM_TransBackGnd:
			if(ToggleMenuCheckMark(g_hPopupMenu,id)) 
			{
				bTransBack=false;
				ResetBackGround(hwnd);
			}
			else
				bTransBack=true;
			break;
		case IDM_ShowSeconds :
			if(ToggleMenuCheckMark(g_hPopupMenu,id)) 
			{
				bShowSeconds=false;
				bSmothMovement=false;
				SetMenuState(g_hPopupMenu,IDM_SmothMovement,MFS_DISABLED);
			}
			else
			{
				bShowSeconds=true;
				SetMenuState(g_hPopupMenu,IDM_SmothMovement,MFS_ENABLED);
			}
			
			break;
		case IDM_SmothMovement:
			if(ToggleMenuCheckMark(g_hPopupMenu,id)) 
				bSmothMovement=false;
			else
				bSmothMovement=true;
			break;
		case IDM_TransFull:
				iTranslucency=10;
				ToggleMenuCheckMark(g_hPopupMenu,id);
				SetMenuState(g_hPopupMenu,IDM_TransOpaque,MFS_UNCHECKED);
				SetMenuState(g_hPopupMenu,IDM_TransCustom,MFS_UNCHECKED);
				SetLayeredWindowAttributes(hwnd,GetColorAtPixel(g_hBitMap),iTranslucency,LWA_COLORKEY|LWA_ALPHA);
				break;
		case IDM_TransOpaque:
				iTranslucency=255;
				ToggleMenuCheckMark(g_hPopupMenu,id);
				SetMenuState(g_hPopupMenu,IDM_TransFull,MFS_UNCHECKED);
				SetMenuState(g_hPopupMenu,IDM_TransCustom,MFS_UNCHECKED);
				SetLayeredWindowAttributes(hwnd,GetColorAtPixel(g_hBitMap),iTranslucency,LWA_COLORKEY|LWA_ALPHA);
				break;
		case IDM_TransCustom:
				iChoice=ShowDialog(IDD_TRANSPARENCY,hwnd,iTranslucency);
				if(iChoice)
				{
					SetMenuState(g_hPopupMenu,IDM_TransFull,MFS_UNCHECKED);
					SetMenuState(g_hPopupMenu,IDM_TransOpaque,MFS_UNCHECKED);
				}
				SetLayeredWindowAttributes(hwnd,GetColorAtPixel(g_hBitMap),iTranslucency,LWA_COLORKEY|LWA_ALPHA);
				break;
		case IDM_SetColor:
				ShowDialog(IDD_COLORCHOOSER,hwnd,(BYTE &)iChoice);
				break;
		case IDM_Calendar:
				ShowDialog(IDD_CALENDAR,hwnd,(BYTE &)iChoice);
				break;
		
	}

}
void OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
	//if command is a system command
	if (!IsCustomMenuID(g_hPopupMenu, cmd))
		if (cmd==IDM_About)
			MessageBox(hwnd,TEXT("Analog Clock by Adarsha M V \n Please visit http://adarsha.mv.googlepages.com \n to get the latest vesrion and more.."),TEXT("About Ady's Analog Clock"),MB_OK|MB_ICONINFORMATION);
		else
			//pass the commands to Default Window Proc
			DefWindowProc(hwnd, WM_SYSCOMMAND, cmd ,MAKELPARAM(x,y));
	else
		PostMessage(hwnd,WM_COMMAND,MAKEWPARAM(cmd,1),0);

}

void OnMove(HWND hwnd, int x, int y)
{
	//PostMessage(hwnd,WM_ERASEBKGND,0,0);
}



bool OnEraseBkgnd(HWND hwnd, HDC hdc)
{
	if(bTransBack)
	{
		HBRUSH hNewBkGnd,hOldBkGnd;
		RECT rect;

		hNewBkGnd=GetTransBkGndBrush(hwnd,g_hRgn);
		hOldBkGnd=(HBRUSH)SelectObject(hdc,hNewBkGnd);

		// Erase the area needed
		GetClipBox(hdc,&rect);		
		PatBlt(hdc,rect.left, rect.top, rect.right-rect.left+1 , rect.bottom-rect.top+1 ,PATCOPY);
		
		SelectObject(hdc,hOldBkGnd);
		DeleteObject(hNewBkGnd);
		return true;
	}
	if (DefWindowProc(hwnd, WM_ERASEBKGND, (WPARAM)(HDC)hdc ,0L))
		return true;
	else
		return false;
}


