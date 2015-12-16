#include "StdAfx.h"
#include "Calendar.h"
#include "Globals.h"
#include "resource.h"
#include "ColorFunctions.h"
#include "DateFunctions.h"

SYSTEMTIME sCurrentDay; 
HBITMAP hBackGndBitmap=NULL;
HMENU hPopUpMenu;
HWND hToolTipWindow;

WNDPROC hOldProc=NULL;

RECT *rDateRects=NULL;
int iPreviousHighlight=-1;

bool ShowCalendar(HWND hwndParent)
{
     if(DialogBox(g_hInstance,MAKEINTRESOURCE(IDD_CALENDAR),hwndParent,(DLGPROC)CalendarDialogProc))
		 return true;
	 else
		 return false;
}

LRESULT WINAPI SubClassProc(HWND hwndControl, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) 
	{
		case WM_KEYDOWN:
			{
				switch(wParam)
				{
				case VK_DOWN:
						sCurrentDay=DateAdd(dd,7,sCurrentDay); 
						InvalidateRect(GetParent(hwndControl),NULL,FALSE);
						break;
				case VK_UP:
						sCurrentDay=DateAdd(dd,-7,sCurrentDay); 
						InvalidateRect(GetParent(hwndControl),NULL,FALSE);
						break;
				case VK_LEFT:
						sCurrentDay=DateAdd(dd,-1,sCurrentDay); 
						InvalidateRect(GetParent(hwndControl),NULL,FALSE);
						break;
				case VK_RIGHT:
						sCurrentDay=DateAdd(dd,1,sCurrentDay); 
						InvalidateRect(GetParent(hwndControl),NULL,FALSE);
						break;
				}
			}
			break;
		case WM_ERASEBKGND:
			return 1;
		case WM_GETDLGCODE:
			return CallWindowProc(hOldProc,hwndControl,uMsg,wParam,lParam)|DLGC_WANTARROWS;
			break;
    }
	return CallWindowProc(hOldProc,hwndControl,uMsg,wParam,lParam);
}


INT_PTR CALLBACK CalendarDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
	{
		HANDLE_MSG(hwndDlg, WM_INITDIALOG,	OnCalendarInitDialog);    		
		HANDLE_MSG(hwndDlg, WM_CLOSE,		OnCalendarClose);
		HANDLE_MSG(hwndDlg, WM_PAINT,		OnCalendarPaint);	
		HANDLE_MSG(hwndDlg, WM_COMMAND,		OnCalendarCommand);
		HANDLE_MSG(hwndDlg, WM_LBUTTONDOWN, OnCalendarLButtonDown);
		HANDLE_MSG(hwndDlg, WM_LBUTTONUP,	OnCalendarLButtonUp );
		HANDLE_MSG(hwndDlg, WM_RBUTTONUP,	OnCalendarRButtonUp);
		HANDLE_MSG(hwndDlg, WM_MOUSEMOVE,	OnCalendarMouseMove);
		HANDLE_MSG(hwndDlg, WM_NCMOUSEMOVE,	OnCalendarNCMouseMove);
		//don't paint backgnd
		case WM_ERASEBKGND:
			return 1;

	}

return FALSE;
}


void OnCalendarNCMouseMove(HWND hwndDlg, int x, int y, UINT codeHitTest)
{
	//revert back for previous highlight
	HDC hDC=GetDC(hwndDlg);
	if (iPreviousHighlight==31)
		HeighlightText(hDC,TEXT("X"),rDateRects[iPreviousHighlight],clrCalendarDate/3,false,1.3,false);
	else if (iPreviousHighlight>=32)		
	{
		char sDay[25];
		wsprintf(sDay,"%d-%s-%d",sCurrentDay.wDay,sMonthNames[sCurrentDay.wMonth-1],sCurrentDay.wYear);
		HeighlightText(hDC,sDay,rDateRects[32],clrCalendarDate/6,false,1.0,false);	
	}
	else
		HeighlightDate(hDC,iPreviousHighlight,rDateRects[iPreviousHighlight],clrCalendarDate,false);
	iPreviousHighlight=-1;
	ReleaseDC(hwndDlg,hDC);
}

void OnCalendarMouseMove(HWND hwndDlg, int x, int y, UINT keyFlags)	
{
	int i=0;
	if (!(keyFlags&MK_LBUTTON))
	{
		POINT pPoint;HDC hDC;
		pPoint.x=x;
		pPoint.y=y;
		//if still within old select area then simply exit
		if(iPreviousHighlight>=0 && PtInRect(&rDateRects[iPreviousHighlight],pPoint))
			return;
		if(PtInRect(&rDateRects[31],pPoint)&&iPreviousHighlight==-1)
		{					
			hDC=GetDC(hwndDlg);
			HeighlightText(hDC,TEXT("X"),rDateRects[31],clrCalendarDate/3,true,1.3);						
			Draw3DBorder(hDC,rDateRects[31],clrCalendarBackGnd);
			iPreviousHighlight=31;
			ReleaseDC(hwndDlg,hDC);
			return;
		}

		if(PtInRect(&rDateRects[32],pPoint)&&iPreviousHighlight==-1)
		{					
			char sDay[25];
			hDC=GetDC(hwndDlg);
			wsprintf(sDay,"%d-%s-%d",sCurrentDay.wDay,sMonthNames[sCurrentDay.wMonth-1],sCurrentDay.wYear);
			HeighlightText(hDC,sDay,rDateRects[32],clrCalendarDate/6,true);	
			Draw3DBorder(hDC,rDateRects[32],clrCalendarBackGnd);
			iPreviousHighlight=32;
			ReleaseDC(hwndDlg,hDC);
			return;
		}

		for (i=0;i<31;i++)
			if(PtInRect(&rDateRects[i],pPoint))
			{
				//revert back for previous highlight
				if(iPreviousHighlight>=0&&iPreviousHighlight!=i)
				{
					hDC=GetDC(hwndDlg);
					if (iPreviousHighlight==31)
						HeighlightText(hDC,TEXT("X"),rDateRects[iPreviousHighlight],clrCalendarDate/3,false,1.3,false);
					else if (iPreviousHighlight>=32)		
					{
						char sDay[25];
						wsprintf(sDay,"%d-%s-%d",sCurrentDay.wDay,sMonthNames[sCurrentDay.wMonth-1],sCurrentDay.wYear);
						HeighlightText(hDC,sDay,rDateRects[32],clrCalendarDate/6,false,1.0,false);	
					}
					else
						HeighlightDate(hDC,iPreviousHighlight,rDateRects[iPreviousHighlight],clrCalendarDate,false);
					iPreviousHighlight=-1;
					ReleaseDC(hwndDlg,hDC);

				}
			
				hDC=GetDC(hwndDlg);
				HeighlightDate(hDC,i,rDateRects[i],clrCalendarDate,true);						
				iPreviousHighlight=i;
				ReleaseDC(hwndDlg,hDC);
				return;
			}

		if (i>=31 && iPreviousHighlight>=0 &&(pPoint.x<rDateRects[iPreviousHighlight].left-2||pPoint.x>rDateRects[iPreviousHighlight].right+2||pPoint.y<rDateRects[iPreviousHighlight].top-2||pPoint.y>rDateRects[iPreviousHighlight].bottom+2))
		{
			//revert back for previous highlight
			hDC=GetDC(hwndDlg);
			if (iPreviousHighlight==31)
				HeighlightText(hDC,TEXT("X"),rDateRects[iPreviousHighlight],clrCalendarDate/3,false,1.3,false);
			else if (iPreviousHighlight>=32)		
			{
				char sDay[25];
				wsprintf(sDay,"%d-%s-%d",sCurrentDay.wDay,sMonthNames[sCurrentDay.wMonth-1],sCurrentDay.wYear);
				HeighlightText(hDC,sDay,rDateRects[32],clrCalendarDate/6,false,1.0,false);	
			}
			else
				HeighlightDate(hDC,iPreviousHighlight,rDateRects[iPreviousHighlight],clrCalendarDate,false);
			iPreviousHighlight=-1;
			ReleaseDC(hwndDlg,hDC);
		}
	}
	else if(keyFlags&MK_LBUTTON)
	{
		if (iPreviousHighlight!=-1)
			InvalidateRect(hwndDlg,NULL,FALSE);
		else
			PostMessage(hwndDlg,WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(x,y));
	}
	return;
}


void OnCalendarLButtonUp(HWND hwndDlg, int x, int y, UINT flags) 
{
	POINT pPoint;
	pPoint.x=x;
	pPoint.y=y;
	//exit
	if(PtInRect(&rDateRects[31],pPoint))
		PostMessage(hwndDlg,WM_CLOSE,0,0);

	if(iPreviousHighlight!=-1 && PtInRect(&rDateRects[iPreviousHighlight],pPoint))
	{
		if (iPreviousHighlight!=32)
		{
			sCurrentDay.wDay=iPreviousHighlight+1;
			InvalidateRect(hwndDlg,NULL,FALSE);
		}
		else
		{					
			char sDay[25];int i;
			HDC hDC;HMENU hYearMenu;
			hDC=GetDC(hwndDlg);
			wsprintf(sDay,"%d-%s-%d",sCurrentDay.wDay,sMonthNames[sCurrentDay.wMonth-1],sCurrentDay.wYear);
			HeighlightText(hDC,sDay,rDateRects[32],clrCalendarDate/6,true);
			Draw3DBorder(hDC,rDateRects[32],clrCalendarBackGnd);
			iPreviousHighlight=32;
			ReleaseDC(hwndDlg,hDC);
			//Create a Popup menu showing +/- 5 years
			hYearMenu=CreatePopupMenu();

			for (i=0;i<10;i++)
			{
				wsprintf(sDay,"%d",sCurrentDay.wYear+i-5); 
				InsertMenu(hYearMenu,i,MF_BYPOSITION|MF_POPUP|MF_STRING,i+5000,sDay);
			}
			ClientToScreen(hwndDlg,&pPoint);
			SetMenuDefaultItem(hYearMenu,5,TRUE);
			TrackPopupMenu(hYearMenu,TPM_LEFTBUTTON,pPoint.x,pPoint.y,0,hwndDlg,NULL);
			i=GetInputState();
		}

	}
	return;
}

void OnCalendarRButtonUp(HWND hwndDlg, int x, int y, UINT keyFlags)
{
	HMENU hMenu; POINT MenuCoord;
	hMenu=GetSubMenu(hPopUpMenu, 0); 
	//x,y are relative to 0,0 of the window, but we need relative to 0,0 of the screen to display the menu
	MenuCoord.x=x;
	MenuCoord.y=y;
	ClientToScreen(hwndDlg,&MenuCoord);
	TrackPopupMenu(hMenu,TPM_LEFTBUTTON,MenuCoord.x,MenuCoord.y,0,hwndDlg,NULL);
}


void OnCalendarLButtonDown(HWND hwndDlg, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if(iPreviousHighlight!=-1)
	{
		HDC hDC;
		hDC=GetDC(hwndDlg);				
		Draw3DBorder(hDC,rDateRects[iPreviousHighlight],clrCalendarBackGnd,true); 
		ReleaseDC(hwndDlg,hDC);
	}
}
void OnCalendarCommand(HWND hwndDlg, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id) // This switch identifies the control
	{
	//Enter Key
	case 1:
		EndDialog(hwndDlg,TRUE);
		return;
	//Escape Key
	case 2:
		EndDialog(hwndDlg,FALSE);
		return;
	}
	if(id>=5000)
	{ sCurrentDay=DateAdd(yy,id-5005,sCurrentDay);
		InvalidateRect(hwndDlg,NULL,FALSE);
		return;
	}

}


void OnCalendarPaint(HWND hwndDlg)
{
	//refresh 
	PAINTSTRUCT ps;
	BeginPaint(hwndDlg, &ps);			
	DrawMonth(hwndDlg,ps.hdc); 
	EndPaint(hwndDlg, &ps);
	return;
}


BOOL OnCalendarInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lParam)
{
	//Initialize common controls
	INITCOMMONCONTROLSEX InitCtrlEx;
	HRGN hRgn;RECT rRect;
	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC  =  ICC_BAR_CLASSES;
	InitCommonControlsEx(&InitCtrlEx);
	GetLocalTime (&sCurrentDay);		
	GetClientRect(hwndDlg,&rRect);
	//GetWindowRect(hwndDlg,&rRect);
	rRect.left+=GetSystemMetrics(SM_CXDLGFRAME);
	rRect.top+=GetSystemMetrics(SM_CYSIZE)+GetSystemMetrics(SM_CYDLGFRAME);
	rRect.right+=GetSystemMetrics(SM_CXDLGFRAME);
	rRect.bottom+=GetSystemMetrics(SM_CYSIZE)+GetSystemMetrics(SM_CYDLGFRAME);

	hRgn=CreateRoundRectRgn(rRect.left,rRect.top,rRect.right,rRect.bottom,20,20);
	SetWindowRgn(hwndDlg,hRgn,TRUE);
	DeleteObject(hRgn);

	hPopUpMenu=LoadMenu(g_hInstance,MAKEINTRESOURCE(IDR_CalendarMenu));

	hOldProc=(WNDPROC)SetWindowLong(GetDlgItem(hwndDlg,IDC_Calendar),GWL_WNDPROC,(LONG)SubClassProc); 

	rDateRects=new RECT[33];

	hToolTipWindow= CreateWindowEx(WS_EX_TRANSPARENT, TOOLTIPS_CLASS, NULL,WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT,hwndDlg, NULL, g_hInstance,NULL);
	SetWindowPos(hToolTipWindow, HWND_TOPMOST,0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	return TRUE;
}


void OnCalendarClose(HWND hwndDlg)
{
	DeleteObject(hBackGndBitmap);
	delete rDateRects;
	rDateRects=NULL;
	hBackGndBitmap=NULL;
	if(hOldProc!=NULL)
	{
		SetWindowLong(GetDlgItem(hwndDlg,IDC_Calendar),GWL_WNDPROC,(LONG)hOldProc); 
		hOldProc=NULL;
	}
	DestroyMenu(hPopUpMenu);
	return;
}




HBITMAP GetCalendarBackGnd(HWND hwndDlg,HDC hDC,COLORREF clrDrawingColor)
{
	RECT CurrentPos;HBRUSH hBrush; HPEN hPen;TOOLINFO ti;

	HDC hMemDC;
	HBITMAP hMemBitMap,hOldBitmap;	
	int Width,Height,Top,Left,iRatioX,iRatioY;
	
	ti.cbSize=sizeof(ti);
	ti.uFlags = TTF_TRANSPARENT | TTF_SUBCLASS;
	ti.hwnd = hwndDlg;
	
	GetClientRect(hwndDlg,&CurrentPos);
	Left=CurrentPos.left;
	Top=CurrentPos.top;
	Width=CurrentPos.right-CurrentPos.left+1;
	Height=CurrentPos.bottom-CurrentPos.top+1;

	hMemDC = CreateCompatibleDC(hDC);	
	hMemBitMap= CreateCompatibleBitmap(hDC, Width, Height);
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hMemBitMap);
	
	iRatioX=Width/50;
	iRatioY=Height/50;

	//clear all
	hBrush=CreateSolidBrush(clrDrawingColor);
	FillRect(hMemDC,&CurrentPos,hBrush);
	DeleteObject(hBrush);
	
	//draw gradiant backgnd
	for (int i=0;i<=25;i++)
	{
		hBrush=CreateSolidBrush(ColorAdjustLuma(clrDrawingColor,i*10));
		SelectObject(hMemDC,hBrush); 
		hPen=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawingColor,i*10));
		SelectObject(hMemDC,hPen);
		RoundRect(hMemDC,iRatioX*i ,iRatioY*i ,Width-iRatioX*i,Height-iRatioY*i,25,25);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}

	POINT pPoint;

	rDateRects[31].top=3;
	rDateRects[31].bottom=18;
	rDateRects[31].left=Width-30;
	rDateRects[31].right=Width-10;
	//add a tooltip
	ti.uId = 0;
	ti.lpszText = TEXT("Close");
	ti.rect = rDateRects[31];
	SendMessage(hToolTipWindow, TTM_ADDTOOL, 0, (LPARAM)&ti);
	
	pPoint.x=long(rDateRects[31].left+(rDateRects[31].right-rDateRects[31].left)/2);
	pPoint.y=long(rDateRects[31].top+(rDateRects[31].bottom-rDateRects[31].top)/2);	
	DrawText(hMemDC,TEXT("X"),pPoint,clrCalendarDate/3,1.3);

	rDateRects[32].top=2;
	rDateRects[32].bottom=20;
	rDateRects[32].left=Width/2-40;
	rDateRects[32].right=Width/2+40;
	//add a tooltip
	ti.uId = 1;
	ti.lpszText = TEXT("Change Current Calendar Year");
	ti.rect = rDateRects[32];
	SendMessage(hToolTipWindow, TTM_ADDTOOL, 0, (LPARAM)&ti);
	
	SelectObject(hMemDC,hOldBitmap);
	DeleteDC(hMemDC);

	return hMemBitMap; 
}



void DrawMonth(HWND hwndDlg,HDC hDC)
{
	RECT rRect,rClientRect;
	int iSaveDCState,iX,iY,iRowHeight,iRowWidth;
	COLORREF clrColor=clrCalendarDate;
	int iDay,iLastDay; SYSTEMTIME dDay;
	char sDay[25];POINT pPoint;

	HDC hMemDC,hTempDC;HBITMAP hBackGnd,hOldBitmap,hOldBitmap1;

	iSaveDCState=SaveDC(hDC);

	GetClientRect(hwndDlg,&rClientRect);

	//Get a plain backgnd
	if (hBackGndBitmap==NULL)
		hBackGndBitmap=GetCalendarBackGnd(hwndDlg,hDC,clrCalendarBackGnd);
	
	hTempDC=CreateCompatibleDC(hDC);
	hOldBitmap1=(HBITMAP)SelectObject(hTempDC,hBackGndBitmap); 

	hMemDC=CreateCompatibleDC(hDC);		
	hBackGnd=CreateCompatibleBitmap(hDC,rClientRect.right-rClientRect.left,rClientRect.bottom-rClientRect.top);
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBackGnd);

	BitBlt(hMemDC,rClientRect.left,rClientRect.top,rClientRect.right,rClientRect.bottom,hTempDC,0,0,SRCCOPY);
	SelectObject(hTempDC,hOldBitmap1); 
	DeleteDC(hTempDC);

	//Draw the Calendar on the memory DC
	GetWindowRect(GetDlgItem(hwndDlg,IDC_Calendar),&rRect);	
	MapWindowPoints(NULL,hwndDlg,(POINT*)&rRect,2);

	Draw3DBorder(hMemDC,rRect,clrCalendarBackGnd,false); 

	iRowWidth=(rRect.right-rRect.left)/7;
	iRowHeight=(rRect.bottom-rRect.top)/7;

	
	for (iX=0;iX<7;iX++)
	{
		pPoint.x=long(iX*iRowWidth+iRowWidth);
		pPoint.y=long(1.9*iRowHeight);
		DrawText(hMemDC,sWeekNames[iX],pPoint,clrColor,1.0);
	}

	dDay=sCurrentDay;
	iLastDay=DaysInMonth(dDay);


	for (iY=1,iDay=0;iY<7;iY++)
	{
		for (iX=(iY==1?FirstOftheMonth(dDay):0);iX<7 && iDay<iLastDay;iX++,iDay++)
		{
			//pPoint.x=long(iX*iRowWidth+iRowWidth);
			//pPoint.y=long(iY*iRowHeight+iRowHeight*1.9);			
			rDateRects[iDay].top=long(iY*iRowHeight+iRowHeight*1.4);
			rDateRects[iDay].left=long(iX*iRowWidth+iRowWidth/1.5);
			rDateRects[iDay].bottom=rDateRects[iDay].top+iRowHeight;
			rDateRects[iDay].right=long(rDateRects[iDay].left+iRowWidth/1.5);
			pPoint.x=long(rDateRects[iDay].left+(rDateRects[iDay].right-rDateRects[iDay].left)/2);
			pPoint.y=long(rDateRects[iDay].top+(rDateRects[iDay].bottom-rDateRects[iDay].top)/2);
			wsprintf(sDay,"%d",iDay+1);
			DrawText(hMemDC,sDay,pPoint,clrColor,1.0);		
		}
	}

	//Heilight current day
	dDay=sCurrentDay;
	
/*
	for (iY=1,iDay=(8-FirstOftheMonth(dDay));iY<7;iY++,iDay+=7)
	{
		if (iDay>dDay.wDay) 
			break;
	}		
	rRect.top=long((iY)*iRowHeight+iRowHeight*1.4);
	rRect.left=long((DayOfTheWeek(dDay))*iRowWidth+iRowWidth/1.5);
	rRect.bottom=rRect.top+iRowHeight;
	rRect.right=long(rRect.left+iRowWidth/1.5);
	//give some 3D 
	Draw3DBorder(hMemDC,rRect,clrCalendarBackGnd);
*/
	Draw3DBorder(hMemDC,rDateRects[dDay.wDay-1],clrCalendarBackGnd);
	
	//Print the date dd-mm-yyyy
	pPoint.x=long(rDateRects[32].left+(rDateRects[32].right-rDateRects[32].left)/2);
	pPoint.y=long(rDateRects[32].top+(rDateRects[32].bottom-rDateRects[32].top)/2);
	wsprintf(sDay,"%d-%s-%d",dDay.wDay,sMonthNames[dDay.wMonth-1],dDay.wYear);
	DrawText(hMemDC,sDay,pPoint,clrColor/6,1.0);			

	//Blt it to the real DC
	BitBlt(hDC,rClientRect.left,rClientRect.top,rClientRect.right-rClientRect.left,rClientRect.bottom-rClientRect.top,hMemDC,0,0,SRCCOPY);  

	//clean up and return
	RestoreDC(hDC,iSaveDCState);
	SelectObject(hMemDC,hOldBitmap);
	DeleteObject(hBackGnd);
	DeleteDC(hMemDC);

}

void HeighlightDate(HDC hDC,int iDay,RECT rBoundingRect,COLORREF clrColor, bool bHighlight)
{
	char sDay[3];

	wsprintf(sDay,"%d",iDay+1);
	HeighlightText(hDC,sDay,rBoundingRect,clrColor,bHighlight,1.0,true); 
}

void HeighlightText(HDC hDC,LPCTSTR sText,RECT rBoundingRect,COLORREF clrColor, bool bHighlight,double dSize,bool bLeaveBorder)
{
	POINT pPoint;

	HDC hMemDC;HBITMAP hOldBitmap;

	hMemDC=CreateCompatibleDC(hDC);		
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBackGndBitmap);

	//Blt the background to the real DC
	
	if(bLeaveBorder)
		BitBlt(hDC,rBoundingRect.left+2,rBoundingRect.top+2,rBoundingRect.right-rBoundingRect.left-2,rBoundingRect.bottom-rBoundingRect.top-2,hMemDC,rBoundingRect.left+2,rBoundingRect.top+2,SRCCOPY);  
	else
		BitBlt(hDC,rBoundingRect.left,rBoundingRect.top,rBoundingRect.right-rBoundingRect.left+1,rBoundingRect.bottom-rBoundingRect.top+1,hMemDC,rBoundingRect.left,rBoundingRect.top,SRCCOPY);  
	
	pPoint.x=long(rBoundingRect.left+(rBoundingRect.right-rBoundingRect.left)/2);
	pPoint.y=long(rBoundingRect.top+(rBoundingRect.bottom-rBoundingRect.top)/2);
	DrawText(hDC,sText,pPoint,(bHighlight?ColorAdjustLuma(clrColor,500):clrColor),dSize);
	SelectObject(hMemDC,hOldBitmap);
	DeleteDC(hMemDC);
}
