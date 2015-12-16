
LRESULT CALLBACK SubClassProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CalendarDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool ShowCalendar(HWND hWnd);
void DrawMonth(HWND hwndDlg,HDC hDC);
HBITMAP GetCalendarBackGnd(HWND hwndDlg,HDC hDC,COLORREF clrDrawingColor);
void HeighlightDate(HDC hDC,int iDay,RECT rBoundingRect,COLORREF clrColor, bool bHighlight=true);
void HeighlightText(HDC hDC,LPCTSTR sText,RECT rBoundingRect,COLORREF clrColor, bool bHighlight=true,double dSize=1.0,bool bLeaveBorder=true);


BOOL OnCalendarInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam); 
void OnCalendarClose(HWND hwnd);
void OnCalendarPaint(HWND hwnd); 
void OnCalendarCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnCalendarLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void OnCalendarLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
void OnCalendarRButtonUp(HWND hwnd, int x, int y, UINT flags); 
void OnCalendarMouseMove(HWND hwnd, int x, int y, UINT keyFlags);	
void OnCalendarNCMouseMove(HWND hwnd, int x, int y, UINT codeHitTest);
