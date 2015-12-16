#include "StdAfx.h"

COLORREF ColorAdjustLuma(COLORREF clrRGB,int n)
{
	double dLuminance,dRatio, dRed,dGreen,dBlue,dMax,dMin;
	BYTE bRed,bBlue,bGreen;

	n=(n<-1000)?-1000:(n>1000)?1000:n;

	bRed = GetRValue(clrRGB);
    bGreen = GetGValue(clrRGB);
    bBlue = GetBValue(clrRGB);
	
	dRed = GetRValue(clrRGB)/255.0;
	dGreen=GetGValue(clrRGB)/255.0;
	dBlue=GetBValue(clrRGB)/255.0;
	
	dMax=(dRed>=dGreen && dRed>=dBlue?dRed:(dGreen>=dBlue?dGreen:dBlue));
	dMin=(dRed<dGreen && dRed<dBlue?dRed:(dGreen<dBlue?dGreen:dBlue));

	
	//Get the current luminance ratio with respect max luminance=255
	//http://en.wikipedia.org/wiki/HSL_color_space, L=(max+min)/2
	
    //dLuminance = (bRed * 0.299 + bGreen * 0.587 + bBlue * 0.114);
	dLuminance = (dMax+dMin)/2.0;
	

	//The luminance in units of 0.1 percent of the total range i.e 1000 units.
	//dRatio=(n*0.255+dLuminance)/dLuminance;
	//dRatio=(n/1000.0*dLuminance+dLuminance)/dLuminance;
	dRatio=n/1000.0;
	dRed = dRatio*255+bRed*dRed;
	dGreen=dRatio*255+bGreen*dGreen;
	dBlue=dRatio*255+bBlue*dBlue;

	bRed = BYTE((dRed<0)?0:(dRed>255)?255:dRed);
	bGreen =BYTE((dGreen<0)?0:(dGreen>255)?255:dGreen);
	bBlue = BYTE((dBlue<0)?0:(dBlue>255)?255:dBlue);



	return RGB(bRed,bGreen,bBlue);
}


COLORREF ColorAdjustSaturation(COLORREF clrRGB,int n)
{
	double dLuminance,dSaturation,dRatio, dRed,dGreen,dBlue,dMax,dMin;
	BYTE bRed,bBlue,bGreen;

	bRed = GetRValue(clrRGB);
    bGreen = GetGValue(clrRGB);
    bBlue = GetBValue(clrRGB);
	
	dRed = GetRValue(clrRGB)/255.0;
	dGreen=GetGValue(clrRGB)/255.0;
	dBlue=GetBValue(clrRGB)/255.0;
	
	dMax=(dRed>=dGreen && dRed>=dBlue?dRed:(dGreen>=dBlue?dGreen:dBlue));
	dMin=(dRed<dGreen && dRed<dBlue?dRed:(dGreen<dBlue?dGreen:dBlue));

	
	//Get the current luminance ratio with respect max luminance=255
	//http://en.wikipedia.org/wiki/HSL_color_space, L=(max+min)/2
	
    dLuminance = (dMax+dMin)/2.0;

	//If a shade of gray, no chroma...

	if((dMax-dMin)==0)
		dSaturation=0;
	else
	{
		if (dLuminance<0.5)
			dSaturation=(dMax-dMin)/(dMax+dMin);
		else
			dSaturation=(dMax-dMin)/(2-dMax-dMin);
	}


	//dRatio=(n*0.255+dSaturation)/dSaturation;
	dRatio=(n/1000.0*dSaturation+dSaturation)/dSaturation;


	bRed = (BYTE)(bRed*dRatio);
    bGreen =(BYTE)(bGreen*dRatio);
    bBlue = (BYTE)(bBlue*dRatio);

	return RGB(bRed,bGreen,bBlue);
}
COLORREF GetWeightedColor(COLORREF clrLineColor,COLORREF clrBackGndColor,WORD wColorWeight)
{
	double clrGrayLine,clrGrayBack, dWeight;
	BYTE  Rnew,Gnew,Bnew, RLine,GLine,BLine,RBack,GBack,BBack;

    RLine = GetRValue( clrLineColor );
    GLine = GetGValue( clrLineColor );
    BLine = GetBValue( clrLineColor );
    clrGrayLine = RLine * 0.299 + GLine * 0.587 + BLine * 0.114;
	//clrGrayLine = RLine * 0.2126 + GLine * 0.7152 + BLine * 0.0722;

    RBack = GetRValue( clrBackGndColor );
    GBack = GetGValue( clrBackGndColor );
    BBack = GetBValue( clrBackGndColor );
    clrGrayBack = RBack * 0.299 + GBack * 0.587 + BBack * 0.114;

	dWeight= (clrGrayLine<clrGrayBack?wColorWeight ^ 65535:wColorWeight)/65535.0;

	Rnew =(BYTE)(RBack > RLine ? (RBack - RLine )*dWeight + RLine :(RLine-RBack)*dWeight + RBack);
	Gnew =(BYTE)(GBack > GLine ? (GBack - GLine )*dWeight + GLine :(GLine-GBack)*dWeight + GBack);
	Bnew =(BYTE)(BBack > BLine ? (BBack - BLine )*dWeight + BLine :(BLine-BBack)*dWeight + BBack);
    
	clrGrayBack = Rnew * 0.299 + Gnew * 0.587 + Bnew * 0.114;

	return RGB(Rnew,Gnew,Bnew);
}

void DrawText(HDC hDC,LPCTSTR sText,POINT CoOrd,COLORREF DrawColor,double dSizeMultiplier)
{
	
	HFONT hFont;    
	SIZE sSize;    
	long x,y;
    int iLen = strlen(sText);

	int iSize;

	iSize=-int(dSizeMultiplier*11.0); 
	
/*	LOGFONT lf;
	GetObject(GetStockObject(ANSI_FIXED_FONT),sizeof(LOGFONT),&lf); 

	hFont=CreateFont(lf.lfHeight, lf.lfWidth, 
        lf.lfEscapement, lf.lfOrientation, lf.lfWeight, 
        lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet, 
        lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality, 
        lf.lfPitchAndFamily, lf.lfFaceName);
*/
	hFont=CreateFont (iSize, 0, 0, 0, FW_BOLD, false, false, false, 
					OEM_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
					DEFAULT_QUALITY,FIXED_PITCH | FF_MODERN, "Times");


    if (!hFont) return;
    HFONT hfPrev = SelectFont(hDC, hFont);

	//calculate the coordinates for the textout
    GetTextExtentPoint(hDC,sText, iLen, &sSize);
	x=CoOrd.x-sSize.cx/2;
	y=CoOrd.y-sSize.cy/2;

	//text out with transparent background
	SetBkMode(hDC, TRANSPARENT );
	SetTextColor(hDC,DrawColor);
    TextOut(hDC, x,y, sText, iLen);	
    
	SelectFont(hDC, hfPrev);
	DeleteObject(hFont);

}

//give some 3D 
void Draw3DBorder(HDC hDC,RECT rRect,COLORREF clrDrawingColor,bool bSinked)
{
	HPEN hPenHilight,hPenShadow; 
	int iSaveDCState;
	iSaveDCState=SaveDC(hDC);

	if (!bSinked)
	{
		hPenHilight=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawingColor,400));
		hPenShadow=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawingColor,-400));
		
	}
	else
	{
		hPenHilight=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawingColor,-400));
		hPenShadow=CreatePen(PS_SOLID,1,ColorAdjustLuma(clrDrawingColor,400));
	}

	MoveToEx(hDC,rRect.left,rRect.bottom,NULL);
		
	SelectObject(hDC,hPenHilight);
	LineTo(hDC,rRect.left,rRect.top);
	LineTo(hDC,rRect.right,rRect.top);

	SelectObject(hDC,hPenShadow);
	LineTo(hDC,rRect.right,rRect.bottom);
	LineTo(hDC,rRect.left,rRect.bottom);

	DeleteObject(hPenHilight);
	DeleteObject(hPenShadow);

	//clean up and return
	RestoreDC(hDC,iSaveDCState);

}
