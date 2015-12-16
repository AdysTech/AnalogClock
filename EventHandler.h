BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpcs);
void OnDestroy(HWND hwnd);
void CALLBACK InvalidateAndKillTimer(HWND hwnd,UINT uMsg,UINT idTimer,DWORD dwTime);
void PaintContent(HWND hwnd, PAINTSTRUCT *pps);
void OnPaint(HWND hwnd);
void OnPaintClient(HWND hwnd, HDC hdc);
void OnLButtonDown( HWND hwnd, BOOL fDblClk, int x, int y, UINT flags );
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
void OnLButtonUp( HWND hwnd, int x, int y, UINT flags );
void OnRButtonDown( HWND hwnd, BOOL fDblClk, int x, int y, UINT flags );
void OnRButtonUp( HWND hwnd, int x, int y, UINT flags );
void OnContextMenu(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnMoving(HWND hwnd,LPRECT CurrentPos);
void OnMove(HWND hwnd, int x, int y);
void OnSysCommand(HWND hwnd, UINT cmd, int x, int y);
bool OnEraseBkgnd(HWND hwnd, HDC hdc);
void OnInitMenu(HWND hwnd, HMENU hMenu);

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
