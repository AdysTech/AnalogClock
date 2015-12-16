// Region.cpp: Converta bitmap to a Region
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Bitmap.h"
#include "Globals.h"
#include "Clock.h"
#include "Resource.h"
#include "ColorFunctions.h"

HRGN CreateRegionFromBitmap(HBITMAP hBitmap, COLORREF transparentColor)
{
	HRGN hRgn = NULL;
	HRGN hTempRgn = NULL;

	// Check for valid bitmap handle
	if ( hBitmap != NULL )
	{
		// Get bitmap object information
		BITMAP bitmap;
		BITMAPINFO sBmpInfo;
		DWORD dwSize;
		unsigned char* ucBytes;
		int bitsPixel,iIndex,iResult;
		unsigned char red, green, blue;
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);	
		memset( &sBmpInfo, 0, sizeof(BITMAPINFO));

		sBmpInfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
		sBmpInfo.bmiHeader.biWidth        = bitmap.bmWidth;
		sBmpInfo.bmiHeader.biHeight       = bitmap.bmHeight;
		sBmpInfo.bmiHeader.biBitCount     = bitmap.bmBitsPixel;
		sBmpInfo.bmiHeader.biPlanes       = 1;
		sBmpInfo.bmiHeader.biCompression  = BI_RGB;
		//sBmpInfo.bmiHeader.biSizeImage    =	bitmap.bmBitsPixel*bitmap.bmWidth*bitmap.bmHeight/8;
		sBmpInfo.bmiHeader.biSizeImage    =	0;
		sBmpInfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);

		bitsPixel= bitmap.bmBitsPixel / 8;
		// Check bitmap color depth (only 24 or 32 bits per pixel allowed)
		if ( ( bitsPixel == 3 ) || ( bitsPixel == 4 ) )
		{
		
			dwSize = bitmap.bmHeight * bitmap.bmWidthBytes;
			ucBytes = new unsigned char[dwSize];
			memset(ucBytes, 0, dwSize);

			//read the bitmap into buffer
			HDC hScreenDC=GetDC(NULL);			
			SelectObject(hScreenDC, hBitmap);
			//iResult=GetDIBits(hScreenDC,hBitmap,0,sBmpInfo.bmiHeader.biHeight,ucBytes,&sBmpInfo,DIB_RGB_COLORS);
			iResult=GetBitmapBits(hBitmap, dwSize, ucBytes);
//			iResult=GetLastError();
			DeleteDC(hScreenDC);

			//Consider Top Left corner pixel as transperent color if nothing is specified
			if (transparentColor==-1)
				transparentColor=RGB(ucBytes[2],ucBytes[1],ucBytes[0]);

			// Create region from bitmap
			for ( int y=0; y<bitmap.bmHeight; y++ )
			{
				for ( int x=0; x<bitmap.bmWidth; x++ )
				{
					// Get pixel color

					iIndex=y*bitmap.bmWidthBytes + bitsPixel*x;		

					blue = ucBytes[iIndex];
					green = ucBytes[iIndex+1];
					red = ucBytes[iIndex+2];

					// Check transparent color
					if ( RGB(red,green,blue) != transparentColor )
					{
						// Combine regions
						if ( hRgn == NULL )
							hRgn = CreateRectRgn(x, y, x+1, y+1);
						else
						{
							// Delete temporary region
							if ( hTempRgn != NULL )
								DeleteObject(hTempRgn);

							// Create temporary region
							hTempRgn = CreateRectRgn(x, y, x+1, y+1);

							// Combine regions
							CombineRgn(hRgn, hRgn, hTempRgn, RGN_OR);
						}

					}
				}
			}

			// Free bitmap bits
			delete ucBytes;
		}
	}

	// Delete temporary region
	if ( hTempRgn != NULL )
		DeleteObject(hTempRgn);
	
	return hRgn;
}


COLORREF GetColorAtPixel(HBITMAP hBitmap, POINT* PixelPos)
{
		BITMAP bitmap;
		DWORD dwSize;
		unsigned char* ucBytes;
		unsigned char red, green, blue;
		int bitsPixel,iIndex;
		POINT AtPixel;
	
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		
		//clean input pixel to be within the bitmap
		if (PixelPos!=NULL)
		{
			AtPixel=*PixelPos;
			if (AtPixel.x<0)
				AtPixel.x=0;
			else if(AtPixel.x >bitmap.bmWidth)
				AtPixel.x=bitmap.bmWidth;

			if (AtPixel.y<0)
				AtPixel.y=0;
			else if(AtPixel.y >bitmap.bmHeight)
				AtPixel.y=bitmap.bmHeight;
		}
		else
			AtPixel.x=AtPixel.y =0;

		

		dwSize = bitmap.bmHeight * bitmap.bmWidthBytes;
		ucBytes = new unsigned char[dwSize];
		GetBitmapBits(hBitmap, dwSize, ucBytes);
		bitsPixel= bitmap.bmBitsPixel / 8;


						
		// Get pixel color
		iIndex=AtPixel.y *bitmap.bmWidthBytes + bitsPixel*AtPixel.x;		
		blue = ucBytes[iIndex];
		green = ucBytes[iIndex+1];
		red = ucBytes[iIndex+2];

		// Free bitmap bits
		delete ucBytes;

		return RGB(red,green,blue);


}


void ResetBackGround(HWND hwnd)
{
	HBRUSH hOldBackGnd;
	hOldBackGnd=(HBRUSH)SetClassLong(hwnd,GCL_HBRBACKGROUND,(long) g_hBackGndBrush);
	if (hOldBackGnd!=g_hBackGndBrush)
		DeleteObject(hOldBackGnd);
}



HBRUSH GetTransBkGndBrush(HWND hwnd,HRGN hWndRgn)
{
	/*adding some code to get the window back ground*/
	HDC ScreenDC,ScreenShotDC;
//	int ScreenWidth,ScreenHeight;
	HBITMAP ScreenShotBitmap;	
	int Width,Height,Top,Left;
	LOGBRUSH brBackGround;
	HBRUSH hNewBackGnd;
	RECT CurrentPos;

	GetWindowRect(hwnd,&CurrentPos);
	Left=CurrentPos.left;
	Top=CurrentPos.top;
	Width=CurrentPos.right-CurrentPos.left+1;
	Height=CurrentPos.bottom-CurrentPos.top+1;
	

	//get the screen shot
	if (hWndRgn==NULL)
		ScreenDC=GetDC(NULL);
	else
		ScreenDC = GetDCEx(NULL,hWndRgn,DCX_EXCLUDERGN);
	ScreenShotDC = CreateCompatibleDC(ScreenDC);	
	ScreenShotBitmap= CreateCompatibleBitmap(ScreenDC, Width, Height);
	SelectObject(ScreenShotDC, ScreenShotBitmap);

	//get contents below window in to the bitmap
	BitBlt(ScreenShotDC,0,0,Width, Height, ScreenDC,Left,Top,  SRCCOPY);
	
	//Make3DBorder(ScreenShotDC,g_hBitMap,hwnd);  
	Make3DBorder(ScreenShotDC,11,false);
	
	brBackGround.lbStyle = BS_PATTERN;
	brBackGround.lbHatch = (long) ScreenShotBitmap;
	hNewBackGnd=CreateBrushIndirect(&brBackGround); 
	
	ReleaseDC(NULL,ScreenDC);
	DeleteDC(ScreenDC);
	DeleteObject(ScreenShotBitmap);
	DeleteDC(ScreenShotDC);
	return hNewBackGnd;
}


void Make3DBorder(HDC PaintDC,int iBorderThikness,bool bSink)
{

	HPEN hHighlightPen, hShadowPen,hOldPen;

	RECT sOuterRect,sInnerRect;

	COLORREF clrHighlight, clrShadow;

	clrHighlight=RGB(220,220,220);
	clrShadow=RGB(128,128,128);

	sInnerRect.left=CENTRE.x-RADIUS;
	sInnerRect.top=CENTRE.y-RADIUS;
	sInnerRect.right=CENTRE.x+RADIUS;
	sInnerRect.bottom=CENTRE.y+RADIUS;

	sOuterRect.left=CENTRE.x-RADIUS-iBorderThikness;
	sOuterRect.top=CENTRE.y-RADIUS-iBorderThikness;
	sOuterRect.right=CENTRE.x+RADIUS+iBorderThikness;
	sOuterRect.bottom=CENTRE.y+RADIUS+iBorderThikness;

	if (bSink)
	{
		COLORREF clrTemp;
		clrTemp=clrHighlight;
		clrHighlight=clrShadow;
		clrShadow=clrTemp;
	}


	hHighlightPen=CreatePen(PS_SOLID, 1,clrHighlight);
	hShadowPen=CreatePen(PS_SOLID, 1,clrShadow);
	
	hOldPen=(HPEN)SelectObject(PaintDC,hHighlightPen);
	Arc(PaintDC,sInnerRect.left,sInnerRect.top,sInnerRect.right,sInnerRect.bottom,CENTRE.x,sInnerRect.bottom,CENTRE.x,sInnerRect.top);
	Arc(PaintDC,sOuterRect.left,sOuterRect.top,sOuterRect.right,sOuterRect.bottom,CENTRE.x,sOuterRect.top,CENTRE.x,sOuterRect.bottom);

	SelectObject(PaintDC,hShadowPen);
	Arc(PaintDC,sInnerRect.left,sInnerRect.top,sInnerRect.right,sInnerRect.bottom,CENTRE.x,sInnerRect.top,CENTRE.x,sInnerRect.bottom);
	Arc(PaintDC,sOuterRect.left,sOuterRect.top,sOuterRect.right,sOuterRect.bottom,CENTRE.x,sOuterRect.bottom,CENTRE.x,sOuterRect.top);
	
	SelectObject(PaintDC,hOldPen);
	DeleteObject(hShadowPen);
	DeleteObject(hHighlightPen);


}

/*
void Make3DBorder(HDC PaintDC,HBITMAP hBitmapMask,HWND hWnd, bool bSink)
{
	BITMAP bitmap;
	BITMAPINFO sBmpInfo;

	DWORD dwSize;
	unsigned char* ucBytes;
	COLORREF lAvgColor,lPixelColor,lSumColor;
	int iIndex,bitsPixel;
	COLORREF clrHighlight, clrShadow;
	bool bRaised,bPrevoiusNotEdge;
	POINT pPixel;
	RECT rtCurrentPos;
	
	int GX[3][3];
	int GY[3][3];
	int dY,dX, iSumRx,iSumRy,iSumBx,iSumBy,iSumGx,iSumGy,iOffset=0;

	lAvgColor=RGB(15,15,15);

	//3x3 GX Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html 
	GX[0][0] = -1; GX[0][1] = -1; GX[0][2] = -1;
	GX[1][0] =  2; GX[1][1] =  2; GX[1][2] =  2;
	GX[2][0] = -1; GX[2][1] = -1; GX[2][2] = -1;
	//3x3 GY Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html 
	GY[0][0] = -1; GY[0][1] =  2; GY[0][2] = -1;
	GY[1][0] = -1; GY[1][1] =  2; GY[1][2] = -1;
	GY[2][0] = -1; GY[2][1] =  2; GY[2][2] = -1;

	//3x3 GX Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html 
	GX[0][0] = -1; GX[0][1] = -1; GX[0][2] = -1;
	GX[1][0] = -1; GX[1][1] =  8; GX[1][2] = -1;
	GX[2][0] = -1; GX[2][1] = -1; GX[2][2] = -1;
	//3x3 GY Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html 
	GY[0][0] = -1; GY[0][1] = -1; GY[0][2] = -1;
	GY[1][0] = -1; GY[1][1] =  4; GY[1][2] = -1;
	GY[2][0] = -1; GY[2][1] = 1; GY[2][2] = -1;

	GetObject(hBitmapMask, sizeof(BITMAP), &bitmap);	
	memset( &sBmpInfo, 0, sizeof(BITMAPINFO));

	sBmpInfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
	sBmpInfo.bmiHeader.biWidth        = bitmap.bmWidth;
	sBmpInfo.bmiHeader.biHeight       = bitmap.bmHeight;
	sBmpInfo.bmiHeader.biBitCount     = bitmap.bmBitsPixel;
	sBmpInfo.bmiHeader.biPlanes       = 1;
	sBmpInfo.bmiHeader.biCompression  = BI_RGB;
	sBmpInfo.bmiHeader.biSizeImage    =	bitmap.bmBitsPixel*bitmap.bmWidth*bitmap.bmHeight/8;
	
	bitsPixel= bitmap.bmBitsPixel / 8;
	
	dwSize = bitmap.bmHeight * bitmap.bmWidthBytes;
	ucBytes = new unsigned char[dwSize];

	//read the bitmap into buffer
	GetDIBits(PaintDC,hBitmapMask,0,sBmpInfo.bmiHeader.biHeight,ucBytes,&sBmpInfo,DIB_RGB_COLORS );
	
	clrHighlight=RGB(220,220,220);
	clrShadow=RGB(125,125,125);
//	clrHighlight=RGB(225,0,0);
//	clrShadow=RGB(0,0,255);

	if (bSink)
	{
		COLORREF clrTemp;
		clrTemp=clrHighlight;
		clrHighlight=clrShadow;
		clrShadow=clrTemp;
	}


	GetWindowRect(hWnd,&rtCurrentPos);

	//scan line by line, comapre contrast between two pixels, highlight/shadow accordingly if there is a edge
	for ( int Y=0; Y<bitmap.bmHeight-1; Y++ )
	{
				for ( int X=0; X<bitmap.bmWidth-1; X++ )
				{
										
					iSumRx=iSumRy=iSumBx=iSumBy=iSumGx=iSumGy=0;
					
					// image boundaries 
					if(Y==0 || Y==bitmap.bmHeight-1)
						lSumColor= 0;
					else if(X==0 || X==bitmap.bmWidth-1)
						lSumColor = 0;
					// Convolution starts here 
					else   
					{
						//X & Y Gradient approximation
						for(dX=-1; dX<=1; dX++)  
						{
							for(dY=-1; dY<=1; dY++) 
							{
								iIndex=(Y+dY)*bitmap.bmWidthBytes + bitsPixel*(X+dX);
								lPixelColor=RGB(ucBytes[iIndex+2],ucBytes[iIndex+1],ucBytes[iIndex]);

								iSumRx=iSumRx + ucBytes[iIndex+2]* GX[dX+1][dY+1];
								iSumGx=iSumGx + ucBytes[iIndex+1]* GX[dX+1][dY+1];
								iSumBx=iSumBx + ucBytes[iIndex]  * GX[dX+1][dY+1];
								
								
								iSumRy=iSumRy + ucBytes[iIndex+2]* GY[dX+1][dY+1];
								iSumGy=iSumGy + ucBytes[iIndex+1]* GY[dX+1][dY+1];
								iSumBy=iSumBy + ucBytes[iIndex]  * GY[dX+1][dY+1];
								

								//Offset								
								iSumRx=iSumRx + iOffset;
								iSumGx=iSumGx + iOffset;
								iSumBx=iSumBx + iOffset;
								
								iSumRx=iSumRy + iOffset;
								iSumGy=iSumGy + iOffset;
								iSumBy=iSumBy + iOffset;
								
						}
						 }
						 
					//Gradient magnitude approximation
						iSumRx=(iSumRx>255?255:(iSumRx<0?0:iSumRx));
						iSumGx=(iSumGx>255?255:(iSumGx<0?0:iSumGx));
						iSumBx=(iSumBx>255?255:(iSumBx<0?0:iSumBx));
						iSumRy=(iSumRy>255?255:(iSumRy<0?0:iSumRy));
						iSumGy=(iSumGy>255?255:(iSumGy<0?0:iSumGy));
						iSumBy=(iSumBy>255?255:(iSumBy<0?0:iSumBy));
						
						lSumColor =RGB(iSumRx+iSumRy,iSumGx+iSumGy,iSumBx+iSumBy);
					 }
//			SetPixel(PaintDC,X,Y,lSumColor);
			//Check for contrast
//			if ( abs(lAvgColor-lPixelColor)>CONTRASTTHREASHHOLD &&bPrevoiusNotEdge)
			if (lSumColor>lAvgColor)
			//if ((lAvgColor-lSumColor)>0 && lSumColor) 
						{
							//x,y are relative to 0,0 of the window, but we need relative to 0,0 of the screen
							pPixel.x=X;
							pPixel.y=Y;

							//ClientToScreen(hWnd,&pPixel);

							//pPixel.x=(pPixel.x<rtCurrentPos.left?rtCurrentPos.left:(pPixel.x>rtCurrentPos.right?rtCurrentPos.right:pPixel.x));
							//pPixel.y=(pPixel.x<rtCurrentPos.top?rtCurrentPos.top:(pPixel.y>rtCurrentPos.bottom?rtCurrentPos.bottom:pPixel.y));
							

							//ScreenToClient(hWnd,&pPixel);

							if (!bRaised)
								SetPixel(PaintDC,pPixel.x,pPixel.y,clrHighlight);
							else
							  SetPixel(PaintDC,pPixel.x,pPixel.y,clrShadow);
							
						}
					else
						bRaised=!bRaised;
				}
				bRaised=false;
			}

	// Free bitmap bits
	delete ucBytes;


}
*/