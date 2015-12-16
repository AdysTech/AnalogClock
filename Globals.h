#include "RuntimeLibrary.h"

#if !defined(Globals_h_)
#define Globals_h_
//Global variables used across the program to hold handles to some key structures.
extern HINSTANCE g_hInstance;		/*This application's HINSTANCE */
extern HRGN g_hRgn;				/*WindowRegion*/
extern HBITMAP g_hBitMap;		/*Image for Skin*/
extern HMENU g_hPopupMenu;		/*Handle to Menu*/
extern HBRUSH g_hBackGndBrush;	/*Handle to BackGround Brush*/
extern HANDLE g_hMutex;			/*Handle to global Mutex*/
extern SetLayeredWindow SetLayeredWindowPtr;


//Global variables 
//for Colors
//"Hour Numbers","Minute Ticks","Week Name","Date","Second Hand","Minute Hand","Hour Hand"
extern COLORREF clrHourNumber,
				clrMinuteTicks,
				clrWeek,
				clrDate,
				clrSecondHand,
				clrMinuteHand,
				clrHourHand,
				clrCalendarBackGnd,
				clrCalendarDate;


//for Clock
extern POINT CENTRE;
extern int RADIUS;

const char sHourNumbers[12][3]={"12","1","2","3","4","5","6","7","8","9","10","11"};
const char sWeekNames[7][4]={"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};
const char sMonthNames[12][4]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

#endif