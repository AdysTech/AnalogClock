#ifndef OptionDialogs_h
#define OptionDialogs_h
INT_PTR CALLBACK TranslucencyDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ColorChooserDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool ShowDialog(int iDialogId,HWND hWnd,BYTE& InOutParam);
void InitializeSlider(HWND hwndDlg,int SilderID,int iRangeMax,int iRangeMin,int iInitValue,bool bTickMarks);
void UpdatePreviewArea(HWND hwndDlg,HDC hDC,int ControlID,COLORREF clrDrawingColor);

static const char *sColorOptions[]={"Hour Numbers","Minute Ticks","Week Name","Date","Second Hand","Minute Hand","Hour Hand","Calendar Background","Calendar Dates"};

#endif