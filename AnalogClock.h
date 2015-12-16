#pragma once

class AnalogClock
{
private:
	HINSTANCE m_hInstance;		/*This application's HINSTANCE */
	HRGN m_hRgn;				/*WindowRegion*/
	HBITMAP m_hBitMap;			/*Image for Skin*/
	HMENU m_hPopupMenu;			/*Handle to Menu*/
	HANDLE m_hMutex;			/*Mutex to limit one instance*/
	COLORREF clrHourNumber,
			 clrMinuteTicks,
			 clrWeek,
			 clrDate;
public:
	AnalogClock(void);
	~AnalogClock(void);
	HWND CreateClock(HINSTANCE hinst);
};
