//Replacement function for standard ColorAdjustLuma  function 
//defined in http://msdn2.microsoft.com/en-us/library/ms629094.aspx
COLORREF ColorAdjustLuma(COLORREF clrRGB,int n);
COLORREF ColorAdjustSaturation(COLORREF clrRGB,int n);

//for WU antialiased line
COLORREF GetWeightedColor(COLORREF clrLineColor,COLORREF clrBackGndColor,unsigned short Weighting);

//Draws the specified text at specified point as the centre
void DrawText(HDC hDC,LPCTSTR TextToPrint,POINT AtPoint,COLORREF InColor,double FontRatio=1.0);

//Draws a3D border arround a rect
void Draw3DBorder(HDC hDC,RECT rRect,COLORREF clrDrawingColor,bool bSinked=false);
