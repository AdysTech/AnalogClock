#include "StdAfx.h"
#include "Clock.h"
#include "Bitmap.h"
#include "Globals.h"
#include "ColorFunctions.h"

COLORREF clrHourNumber,clrMinuteTicks,clrWeek,clrDate,clrSecondHand,clrMinuteHand,clrHourHand,clrCalendarBackGnd,clrCalendarDate;


POINT GetCoordinateForAngle(int dAngle,int iRadius,bool ZeroDegAtTwelve)
{
	POINT pCoOrdinates;double radAngle, dRadius;
	dRadius=iRadius*1.0;

	//Correct input angle within 0-360
	if(dAngle<0) dAngle=360+dAngle;
	if(dAngle>360) dAngle=dAngle%360;

	//adjust angle for zero degree at 12'o Clock
	//by default it will be at 3'o Clock which is geometrical x axis
	if(ZeroDegAtTwelve)
		dAngle=(dAngle+270)%360;

	//Radian=Angle*PI/180	
	radAngle = dAngle*3.14159265358979323846 /180;
	
	//Polor Distance
	//X=Cos(x)*r, Y=Sin(x)*r
	pCoOrdinates.x = long(cos(radAngle)*dRadius);
	pCoOrdinates.y = long(sin(radAngle)*dRadius);

	//Make these points relative to centre of the circle
	pCoOrdinates.x = pCoOrdinates.x+CENTRE.x;
	pCoOrdinates.y = pCoOrdinates.y+CENTRE.y;

	return pCoOrdinates;
}



void DrawClockFace(HDC hDC)
{
	POINT Pixel; 
	int iNumber,iAngle;

	//SetPixel(hDC,CENTRE.x,CENTRE.y,RGB(200,0,0));
	Ellipse(hDC,CENTRE.x-2,CENTRE.y-2,CENTRE.x+2,CENTRE.y+2);

	//Draw the No
	for (iAngle=0,iNumber=0; iAngle<360; iAngle+=6) 
	{	
		if (iAngle%30==0)
		{
			Pixel=GetCoordinateForAngle(iAngle,int(RADIUS*0.92),true);
			DrawText(hDC,sHourNumbers[iNumber],Pixel,clrHourNumber);
			iNumber=iNumber++;
		}
		else
		{
			Pixel=GetCoordinateForAngle(iAngle,int(RADIUS*0.95),true);
			SetPixel(hDC,Pixel.x,Pixel.y,clrMinuteTicks);
		}
	}
}


void DrawClockHands(HDC hDC,bool bShowSeconds,bool bSoftMove)
{
	SYSTEMTIME SysTime;
	POINT CoOrd;
	
	int iSecond, iMinute, iHour;
	char sDay[10];
	
	//Draw the Clock Circle
	POINT pPixel;
	pPixel.x =13;
	pPixel.y =13;
	pPixel=CENTRE;

	GetLocalTime (&SysTime);	

	//Draw Day Info
	CoOrd= GetCoordinateForAngle(0,int(RADIUS*0.5));	
	wsprintf(sDay,"%d/%d",SysTime.wMonth, SysTime.wDay);
	DrawText(hDC,sDay,CoOrd,clrDate,0.85);

	CoOrd= GetCoordinateForAngle(180,int(RADIUS*0.5));		
	DrawText(hDC,sWeekNames[SysTime.wDayOfWeek],CoOrd,clrWeek,0.9);

	//Draw second hand
	//each second moves 360/60=6 degrees
	//if soft move is enabled then seconds hand will move by one degree for each 167 milli sec

	if (!bSoftMove)
		iSecond=SysTime.wSecond*6;
	else		
		iSecond=SysTime.wSecond*6+int(SysTime.wMilliseconds/167); 

	//Draw Minute hand
	//each minute moves 360/60=6 degree + second/60 to show proper proration
	iMinute=SysTime.wMinute*6+int(SysTime.wSecond/10.0);


	//Draw Hour hand
	//each minute moves 360/60=6 degree + Minute/60 to show proper proration
	if (SysTime.wHour>=12)
	{
		//Change the color of 12 to indicate PM
		CoOrd=GetCoordinateForAngle(0,int(RADIUS*0.92),true);
		DrawText(hDC,sHourNumbers[0],CoOrd,ColorAdjustLuma(clrHourNumber,666));  
		iHour=(SysTime.wHour-12)*30+int(SysTime.wMinute/2.0);
	}
	else
		iHour=(SysTime.wHour)*30+int(SysTime.wMinute/2.0);

	DrawHand(hDC,iHour,int(RADIUS*0.60),3,clrHourHand);
	DrawHand(hDC,iMinute,int(RADIUS*0.80),2,clrMinuteHand);
	if (bShowSeconds)
 		DrawHand(hDC,iSecond,int(RADIUS*0.97),3,clrSecondHand);

}


void DrawHand(HDC hDC, int iAngle,int iRadius,int iThickNess,COLORREF clrLineColor)
{
	POINT CoOrd;		
	CoOrd= GetCoordinateForAngle(iAngle,iRadius,true);	
	DrawAntialiasedLine(hDC,CENTRE,CoOrd,clrLineColor);
	return;

}

void DrawAntialiasedLine(HDC hDC,POINT pFrom,POINT pTo,COLORREF clrLineColor)
{
    int iDirX,iXd,iYd;
	int iX,iY;
	WORD iTemp,iSlope, iPixelWeight,iPixelSlope;
	COLORREF clrBackGround;	
    iPixelSlope = 0;    

	//Make sure the line runs top to bottom
	if(pFrom.y > pTo.y )
	{
		LONG lTemp;
		lTemp = pFrom.y; pFrom.y = pTo.y; pTo.y = lTemp;		
		lTemp = pFrom.x; pFrom.x = pTo.x; pTo.x = lTemp;
	}


	//Width and Height of the line
	iXd = pTo.x-pFrom.x;
	iYd = pTo.y-pFrom.y;
	
	//X Direction
	if(iXd<0)
	{
		iDirX=-1;
		iXd=-iXd; 
	}
	else
		iDirX=1; 
    

	//Horizontal, vertical, and diagonal lines do not require Wu antialiasing because 
	//they pass through the center of every pixel they meet

	if (iYd == 0||iXd == 0||iXd == iYd)
	{	

		if (iYd==0)
			for(iX=0;--iXd>0;iX+=iDirX)
				SetPixel(hDC,pFrom.x+iX,pFrom.y,clrLineColor);
		else if (iXd == 0)
			for(iY=0;--iYd>0;iY++)
				SetPixel(hDC,pFrom.x,pFrom.y+iY,clrLineColor);
		else if (iXd == iYd)
			for(iX=0,iY=0;--iYd>0;iY++,iX+=iDirX)
				SetPixel(hDC,pFrom.x+iX,pFrom.y+iY,clrLineColor);
		return;
		
	}

    if (iYd > iXd)
	{
		//It's an Y-major line;

		SetPixel(hDC,pFrom.x,pFrom.y, clrLineColor);
	    
		iPixelSlope = 0;		
		iSlope =WORD ((DWORD(iXd) << 16) / DWORD(iYd));	
		
		for(iX=0,iY=0;iYd-->0;iY++)		
		{
			iTemp = iPixelSlope;
            iPixelSlope += iSlope;
            if (iPixelSlope <= iTemp) 
				iX += iDirX;

            iPixelWeight =BYTE(iPixelSlope >> 8);
            
            //clrBackGround = GetWeightedColor(clrLineColor,GetPixel(hDC,pFrom.x+iX,pFrom.y+iY),iPixelWeight);
			clrBackGround = GetWeightedColor(clrLineColor,ColorAdjustLuma(clrLineColor,-250),iPixelWeight);
            SetPixel(hDC,pFrom.x+iX,pFrom.y+iY,clrBackGround);            
            //clrBackGround = GetWeightedColor(clrLineColor,GetPixel(hDC, pFrom.x+(iX+iDirX),pFrom.y+iY),iPixelWeight^65535);
			clrBackGround = GetWeightedColor(clrLineColor,ColorAdjustLuma(clrLineColor,-250),iPixelWeight^65535);
            SetPixel(hDC, pFrom.x+(iX+iDirX),pFrom.y+iY,clrBackGround);		
		
        }

        SetPixel(hDC, pTo.x ,pTo.y, clrLineColor );
        return;
    }
	else
	{
		//It's an X-major line;

		SetPixel(hDC, pFrom.x ,pFrom.y, clrLineColor );		
	    
		iPixelSlope = 0;
		iSlope =WORD((DWORD(iYd) << 16) / DWORD(iXd));	
		
		for(iX=0,iY=0;iXd-->0;iX+=iDirX)		
		{
			iTemp = iPixelSlope;
            iPixelSlope += iSlope;
            if (iPixelSlope <= iTemp) 
				iY++;

            iPixelWeight = iPixelSlope>>8;
            
            //clrBackGround =GetWeightedColor(clrLineColor,GetPixel(hDC,pFrom.x+iX,pFrom.y+iY),iPixelWeight) ;
            clrBackGround =GetWeightedColor(clrLineColor,ColorAdjustLuma(clrLineColor,-250),iPixelWeight) ;
			SetPixel(hDC,pFrom.x+iX,pFrom.y+iY,clrBackGround);            
            //clrBackGround = GetWeightedColor(clrLineColor,GetPixel(hDC, pFrom.x+iX,pFrom.y+iY+1),iPixelWeight^65535);
            clrBackGround = GetWeightedColor(clrLineColor,ColorAdjustLuma(clrLineColor,-250),iPixelWeight^65535);
			SetPixel(hDC, pFrom.x+iX,pFrom.y+iY+1,clrBackGround);
		}

        SetPixel(hDC, pTo.x ,pTo.y, clrLineColor );
        return;
	}
}

