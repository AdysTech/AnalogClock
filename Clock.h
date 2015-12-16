/*
Coordinate for the small clock
    Start	Difference	End	Centre
X	13		88			101	56
Y	13		88			101	56


const POINT CENTRE={56,56};
const int RADIUS=44;
*/

/*
Coordinate for the present clock
    Start	Difference	End	Centre
X	14		106			120	53
Y	14		106			120	53
*/

//const POINT CENTRE={67,67};
//const int RADIUS=53;


//Draws the Clock Face and hands
void DrawClockFace(HDC hDC);
void DrawClockHands(HDC hDC,bool bShowSeconds,bool bSoftMove=false);
void DrawHand(HDC hDC, int iAngle,int iRadius,int iThickNess,COLORREF clrLineColor);


//converts given angle in to coordinates
POINT GetCoordinateForAngle(int dAngle,int iRadius,bool ZeroDegAtTwelve=false);


//Draw Antialiased Line 
//For fast antialiasing, Xiaolin Wu invented an algorithm - called by his name Wu Antialiasing. 
//Michael Abrash's Graphics Programming Black Book gives excellent treatment of this algorithm.
//Hugo Elias also has an excellent article on the matter.
//Refer http://www.suchit-tiwari.org/antialias.html
void DrawAntialiasedLine(HDC hDC,POINT pFrom,POINT pTo,COLORREF clrLineColor);
