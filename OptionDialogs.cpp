#include "StdAfx.h"
#include "resource.h"
#include "Globals.h"
#include "OptionDialogs.h"
#include "ColorFunctions.h"
#include "Bitmap.h"

int bTranslucency;
int iRed,iGreen,iBlue;

bool ShowDialog(int iDialogId,HWND hWnd,BYTE& InOutParam)
{
	switch (iDialogId)
	{
	case IDD_TRANSPARENCY:
		bTranslucency=InOutParam;	
		if (DialogBox(g_hInstance,MAKEINTRESOURCE(IDD_TRANSPARENCY),hWnd,(DLGPROC)TranslucencyDialogProc))
		{
			InOutParam=bTranslucency;
			return true;
		}
		else
			return false;
	case IDD_COLORCHOOSER:
		if (DialogBox(g_hInstance,MAKEINTRESOURCE(IDD_COLORCHOOSER),hWnd,(DLGPROC)ColorChooserDialogProc))
		{
			return true;
		}
		else
			return false;
	}
	return false;
}



INT_PTR CALLBACK TranslucencyDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		//Initialize common controls
		INITCOMMONCONTROLSEX InitCtrlEx;
		InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitCtrlEx.dwICC  =  ICC_BAR_CLASSES;
		InitCommonControlsEx(&InitCtrlEx);
		InitializeSlider(hwndDlg,IDC_TransSlider,255,10,bTranslucency,true); 
		return TRUE;
		
	case WM_CLOSE:
		EndDialog(hwndDlg, FALSE);
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam)) // This switch identifies the control
		{
		case ID_OK:
			EndDialog(hwndDlg, TRUE);
			return TRUE;
		case ID_CANCEL:
			EndDialog(hwndDlg, FALSE);
			return FALSE;
		}
		break;
	case WM_HSCROLL:
		{
			bTranslucency=SendMessage(GetDlgItem(hwndDlg,IDC_TransSlider),TBM_GETPOS ,0,0);
			SetLayeredWindowAttributes(GetWindow(hwndDlg,GW_OWNER),GetColorAtPixel(g_hBitMap),bTranslucency,LWA_COLORKEY|LWA_ALPHA);
		}


	}

return FALSE;
}


INT_PTR CALLBACK ColorChooserDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			//Initialize common controls
			INITCOMMONCONTROLSEX InitCtrlEx;
			InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
			InitCtrlEx.dwICC  =  ICC_BAR_CLASSES;
			InitCommonControlsEx(&InitCtrlEx);
			InitializeSlider(hwndDlg,IDC_RedSlider,255,1,0,false); 
			InitializeSlider(hwndDlg,IDC_GreenSlider,255,1,0,false); 
			InitializeSlider(hwndDlg,IDC_BlueSlider,255,1,0,false); 


			HWND hWndComboBox=GetDlgItem(hwndDlg,IDC_Combo);
			//add items to combobox
			for (int i=0;i<9;i++)
				SendMessage(hWndComboBox,CB_ADDSTRING,0L,LPARAM(sColorOptions[i]));			
			//update each item data with its original index, it might have changed while sorting
			for (int i=0;i<9;i++)
				SendMessage(hWndComboBox,CB_SETITEMDATA,SendMessage(hWndComboBox,CB_FINDSTRING,0L,LPARAM(sColorOptions[i])),LPARAM(i));
			
			iRed=0;
			iGreen=0;
			iBlue=0;
			EnableWindow(GetDlgItem(hwndDlg,ID_APPLY),FALSE);
			EnableWindow(GetDlgItem(hwndDlg,ID_OK),FALSE);
			return FALSE;
		}
	case WM_CLOSE:
		EndDialog(hwndDlg, FALSE);
		return FALSE;
	case WM_COMMAND:
		// This switch identifies the control
		switch(LOWORD(wParam)) 
		{
		case ID_OK:
			{
				int iOptionIndex;
				HWND hWndComboBox=GetDlgItem(hwndDlg,IDC_Combo);				
				iOptionIndex=SendMessage(hWndComboBox,CB_GETITEMDATA,SendMessage(hWndComboBox,CB_GETCURSEL,0,0),0);
				//"Hour Numbers","Minute Ticks","Week Name","Date","Second Hand","Minute Hand","Hour Hand"
				WaitForSingleObject(g_hMutex,200); 
				switch(iOptionIndex)
				{
				case 0: clrHourNumber=RGB(iRed,iGreen,iBlue);break;
				case 1: clrMinuteTicks=RGB(iRed,iGreen,iBlue);break;
				case 2: clrWeek=RGB(iRed,iGreen,iBlue);break;
				case 3: clrDate=RGB(iRed,iGreen,iBlue);break;
				case 4: clrSecondHand=RGB(iRed,iGreen,iBlue);break;
				case 5: clrMinuteHand=RGB(iRed,iGreen,iBlue);break;
				case 6: clrHourHand=RGB(iRed,iGreen,iBlue);break;
				case 7: clrCalendarBackGnd=RGB(iRed,iGreen,iBlue);break;
				case 8: clrCalendarDate=RGB(iRed,iGreen,iBlue);break;
				}
				ReleaseMutex(g_hMutex); 
				EndDialog(hwndDlg, TRUE);
				return FALSE;
			}
		case ID_APPLY:
			{
				int iOptionIndex;
				HWND hWndComboBox=GetDlgItem(hwndDlg,IDC_Combo);				
				iOptionIndex=SendMessage(hWndComboBox,CB_GETITEMDATA,SendMessage(hWndComboBox,CB_GETCURSEL,0,0),0);
				//"Hour Numbers","Minute Ticks","Week Name","Date","Second Hand","Minute Hand","Hour Hand"
				WaitForSingleObject(g_hMutex,200); 
				switch(iOptionIndex)
				{
				case 0: clrHourNumber=RGB(iRed,iGreen,iBlue);break;
				case 1: clrMinuteTicks=RGB(iRed,iGreen,iBlue);break;
				case 2: clrWeek=RGB(iRed,iGreen,iBlue);break;
				case 3: clrDate=RGB(iRed,iGreen,iBlue);break;
				case 4: clrSecondHand=RGB(iRed,iGreen,iBlue);break;
				case 5: clrMinuteHand=RGB(iRed,iGreen,iBlue);break;
				case 6: clrHourHand=RGB(iRed,iGreen,iBlue);break;
				case 7: clrCalendarBackGnd=RGB(iRed,iGreen,iBlue);break;
				case 8: clrCalendarDate=RGB(iRed,iGreen,iBlue);break;
				}
				ReleaseMutex(g_hMutex); 
				EnableWindow(GetDlgItem(hwndDlg,ID_APPLY),FALSE);
				return FALSE;
			}

		case ID_CANCEL:
			EndDialog(hwndDlg, FALSE);
			return FALSE;
		case IDC_Combo:
			if (HIWORD(wParam)==CBN_SELENDOK)
			{
				int iOptionIndex;COLORREF clrOldColor;
				iOptionIndex=SendMessage(HWND(lParam),CB_GETITEMDATA,SendMessage(HWND(lParam),CB_GETCURSEL,0,0),0);
				WaitForSingleObject(g_hMutex,200); 
				switch(iOptionIndex)
				{
				case 0: clrOldColor=clrHourNumber;break;
				case 1: clrOldColor=clrMinuteTicks;break;
				case 2: clrOldColor=clrWeek;break;
				case 3: clrOldColor=clrDate;break;
				case 4: clrOldColor=clrSecondHand;break;
				case 5: clrOldColor=clrMinuteHand;break;
				case 6: clrOldColor=clrHourHand;break;
				case 7: clrOldColor=clrCalendarBackGnd;break;
				case 8: clrOldColor=clrCalendarDate;break;
				}

				iRed=GetRValue(clrOldColor);
				iGreen=GetGValue(clrOldColor);
				iBlue=GetBValue(clrOldColor);

				SendMessage(GetDlgItem(hwndDlg,IDC_RedSlider),TBM_SETPOS ,TRUE,iRed);
				SendMessage(GetDlgItem(hwndDlg,IDC_GreenSlider),TBM_SETPOS ,TRUE,iGreen);
				SendMessage(GetDlgItem(hwndDlg,IDC_BlueSlider),TBM_SETPOS ,TRUE,iBlue);
				UpdatePreviewArea(hwndDlg,GetDC(hwndDlg),IDC_ResultPreview,RGB(iRed,iGreen,iBlue));
				EnableWindow(GetDlgItem(hwndDlg,ID_OK),TRUE);
				ReleaseMutex(g_hMutex); 
				return FALSE;
			}
			break;
		}		
	case WM_HSCROLL:
		{
			if (lParam)
			{
				DWORD clrOld=RGB(iRed,iGreen,iBlue);
				switch (GetDlgCtrlID(HWND(lParam)))
				{
					case IDC_RedSlider:
						iRed=SendMessage(HWND(lParam),TBM_GETPOS ,0,0);
						break;
					case IDC_GreenSlider:
						iGreen=SendMessage(HWND(lParam),TBM_GETPOS ,0,0);
						break;
					case IDC_BlueSlider:
						iBlue=SendMessage(HWND(lParam),TBM_GETPOS ,0,0);
						break;
				}
				UpdatePreviewArea(hwndDlg,GetDC(hwndDlg),IDC_ResultPreview,RGB(iRed,iGreen,iBlue));
				if(RGB(iRed,iGreen,iBlue)!=clrOld)
					EnableWindow(GetDlgItem(hwndDlg,ID_APPLY),TRUE);
				return FALSE;
			}		
			break;
		}
	case WM_PAINT:
		{
			//refresh the boxes
			PAINTSTRUCT ps;
			BeginPaint(hwndDlg, &ps);			
			UpdatePreviewArea(hwndDlg,ps.hdc,IDC_ResultPreview,RGB(iRed,iGreen,iBlue));
			EndPaint(hwndDlg, &ps);
			return FALSE;
		}
	case WM_NOTIFY:
		{
			NMHDR tNotifyHeader; NMCUSTOMDRAW tRedrawInfo;
			if (lParam!=NULL)
			{
				tNotifyHeader=*(LPNMHDR)lParam;
				if (tNotifyHeader.code==NM_CUSTOMDRAW)
				{
					tRedrawInfo=*(LPNMCUSTOMDRAW) lParam;
					switch (tRedrawInfo.dwDrawStage)
					{
					case CDDS_PREPAINT:
						// This is the way a dialog box can send back lresults
						SetWindowLong(hwndDlg, DWL_MSGRESULT, CDRF_NOTIFYITEMDRAW); 
						return TRUE;

					case CDDS_ITEMPREPAINT :
						if (tRedrawInfo.dwItemSpec == TBCD_THUMB)
						{
							HBRUSH hBrush;HPEN hPen;
							COLORREF clrDrawColor;int iSaveDCState;			
							switch (tNotifyHeader.idFrom)
							{
								case IDC_RedSlider:
									clrDrawColor=RGB(iRed,0,0);
									break;
								case IDC_GreenSlider:
									clrDrawColor=RGB(0,iGreen,0);
									break;
								case IDC_BlueSlider:
									clrDrawColor=RGB(0,0,iBlue);
									break;
							}
							iSaveDCState=SaveDC(tRedrawInfo.hdc);
							

							//If thumb is selected switch to hatched brush
							if (tRedrawInfo.uItemState && CDIS_SELECTED)
							{
								// create a hatch-patterned pixel pattern for patterned brush
								WORD PatternPixel[8] ={ 0x0055,0x00aa,0x0055,0x00aa,0x0055,0x00aa,0x0055,0x00aa };
								HBITMAP hPattern;
								hPattern=CreateBitmap(8, 8, 1, 1, PatternPixel);
								LOGBRUSH logBrush;

								logBrush.lbStyle = BS_PATTERN;
								logBrush.lbHatch = (long)hPattern;
								hBrush=CreateBrushIndirect(&logBrush);
								SetBrushOrgEx(tRedrawInfo.hdc,tRedrawInfo.rc.right%8, tRedrawInfo.rc.top%8,NULL);
								SetBkColor(tRedrawInfo.hdc,clrDrawColor);
								SetTextColor(tRedrawInfo.hdc,ColorAdjustLuma(clrDrawColor,500));								
							}
							else
								hBrush=CreateSolidBrush(clrDrawColor);

							SelectObject(tRedrawInfo.hdc,hBrush);

							//Draw the thumb
							hPen=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawColor,-333));
							SelectObject(tRedrawInfo.hdc,hPen);
							RoundRect(tRedrawInfo.hdc,tRedrawInfo.rc.left,tRedrawInfo.rc.top,tRedrawInfo.rc.right,tRedrawInfo.rc.bottom,2,2);
							DeleteObject(hPen);

							//give some 3D look for the thumb
							hPen=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawColor,400));
							SelectObject(tRedrawInfo.hdc,hPen);
							MoveToEx(tRedrawInfo.hdc,tRedrawInfo.rc.left,tRedrawInfo.rc.bottom-2,NULL);
							LineTo(tRedrawInfo.hdc,tRedrawInfo.rc.left,tRedrawInfo.rc.top);
							MoveToEx(tRedrawInfo.hdc,tRedrawInfo.rc.left+1,tRedrawInfo.rc.top,NULL);
							LineTo(tRedrawInfo.hdc,tRedrawInfo.rc.right-1,tRedrawInfo.rc.top);

							//clean up and return
							RestoreDC(tRedrawInfo.hdc,iSaveDCState);
							DeleteObject(hPen);
							DeleteObject(hBrush);
							SetWindowLong(hwndDlg, DWL_MSGRESULT, CDRF_SKIPDEFAULT ); 
							return TRUE;
						}
						else
						{
							SetWindowLong(hwndDlg, DWL_MSGRESULT, CDRF_DODEFAULT); 
							return FALSE;
						}
					}
				}
			}
			break;
		}
	}

return FALSE;
}


void InitializeSlider(HWND hwndDlg,int SilderID,int iRangeMax,int iRangeMin,int iInitValue,bool bTickMarks)
{
	HWND hSlider;
	hSlider=GetDlgItem(hwndDlg,SilderID);
	//Set the Tick Marks
	if (bTickMarks)
	{		
		SetWindowLong(hSlider,GWL_STYLE ,GetWindowLong(hSlider,GWL_STYLE)|TBS_AUTOTICKS);		
		SendMessage(hSlider,TBM_SETTICFREQ ,(iRangeMax-iRangeMin*2)/10,(LPARAM) 0L);
	}
	//Set the Range for the slider
	SendMessage(hSlider,TBM_SETRANGE,true,(LPARAM) MAKELONG (iRangeMin,iRangeMax));
	SendMessage(hSlider,TBM_SETPOS,true,(LPARAM) MAKELONG (iInitValue,0));
}

void UpdatePreviewArea(HWND hwndDlg,HDC hDC,int ControlID,COLORREF clrDrawingColor)
{
	RECT rRect;HBRUSH hBrush;int iSaveDCState; HPEN hPen;

	iSaveDCState=SaveDC(hDC);

	GetWindowRect(GetDlgItem(hwndDlg,ControlID),&rRect);
	hBrush=CreateSolidBrush(clrDrawingColor);
	SelectObject(hDC,hBrush); 
	hPen=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawingColor,-400));
	SelectObject(hDC,hPen);
	
	MapWindowPoints(NULL,hwndDlg,(POINT*)&rRect,2);
	Rectangle(hDC,rRect.left,rRect.top,rRect.right+1,rRect.bottom+1);

	//give some 3D look for the thumb
	DeleteObject(hPen);
	hPen=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawingColor,400));
	SelectObject(hDC,hPen);
	MoveToEx(hDC,rRect.left,rRect.bottom-1,NULL);
	LineTo(hDC,rRect.left,rRect.top);
	LineTo(hDC,rRect.right,rRect.top);

	//clean up and return
	RestoreDC(hDC,iSaveDCState);
	DeleteObject(hPen);
	DeleteObject(hBrush);

}
