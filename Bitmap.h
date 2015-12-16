//#include "StdAfx.h"


const COLORREF CONTRASTTHREASHHOLD=RGB(128,128,128);

HRGN CreateRegionFromBitmap(HBITMAP SourceBitmap, COLORREF transparentColor=-1);
COLORREF GetColorAtPixel(HBITMAP hBitmap, POINT* PixelPos=NULL);
void MakeWindowTransparent(HDC hDC,HWND hWindowHandle,HRGN );
void ResetBackGround(HWND hwnd);
HBRUSH GetTransBkGndBrush(HWND hWindowHandle,HRGN hRgnHadle);
void Make3DBorder(HDC,HBITMAP,HWND,bool=false);
void Make3DBorder(HDC,int=11,bool=false);
