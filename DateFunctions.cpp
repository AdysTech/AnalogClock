#include "StdAfx.h"
#include "DateFunctions.h"
//i am using 1721423.5 instead of 2400000.5 to get minimum date as 0/0/0001

double DateToDayNumber(SYSTEMTIME dDate)
{
	long a,y,m; double dJulianday;

	a=int((14-dDate.wMonth)/12);
	y=dDate.wYear + 4800 - a;
	m=dDate.wMonth + 12*a - 3;
	dJulianday= dDate.wDay + (153*m + 2)/5 + 365.0 * y + y/4 - y/100 + y/400 -32045;

	//add fraction for time as well
	dJulianday+=(dDate.wHour-12)/24.0+dDate.wMinute/1440.0+dDate.wSecond/86400.0;
	//dJulianday=round(dJulianday,5);

	//return Modified Julian Day
	return dJulianday - 1721423.5;
}

SYSTEMTIME DayNumberToDate(double dJulianday)
{
	SYSTEMTIME dDate;double dOJD; long lJulianday;

	unsigned long g,dg,c,dc,b,db,a,da,y,m,d,s;

	//get the day of the week
	dDate.wDayOfWeek=long(dJulianday+3)%7;

	//revert back to original JD
	dOJD=(dJulianday<1721423.5)?dJulianday+1721423.5:dJulianday;

	//relative Julian day number j from a Gregorian epoch starting on March 1 -4800
	lJulianday=long(dOJD)+32044;
	//number of days since the beginning of the current cycle of Gregorian quadricentennial(there are exactly 146,097 days per cycle) 
	g=lJulianday/146097;
	dg=lJulianday%146097;
	//compute the number (0 to 4) of Gregorian centennial cycles (there are exactly 36,524 days/cycle)
	c = (dg/36524 + 1) * 3/4;
	dc = dg - c * 36524;
	//compute the number(0 to 24) of Julian quadrennial cycles (there are exactly 1,461 days in 4 years, except for the last cycle which may be incomplete by 1 day) 
	b = dc/1461;
	db = dc % 1461;
	//compute the number(0 to 4) of Roman annual cycles (there are exactly 365 days per Roman annual cycle) since the beginning of the Julian quadrennial cycle
	a = (db/365 + 1) * 3/4;
	da = db - a * 365;
	//number of years since the epoch
	y = g * 400 + c * 100 + b * 4 + a;
	//compute the number of months since March 
	m = (da * 5 + 308)/153 - 2;
	//days past since the beginning of the month
	d = da - (m + 4) * 153/5 + 122;
	
	dDate.wYear =WORD(y - 4800 + (m + 2)/12);
	dDate.wMonth = WORD((m + 2)%12 + 1);
	dDate.wDay= WORD(d + 1);
	

	//goto time coneversion
	//get the fraction, original JulianDay considers 12PM as 0, so add 0.5 to compensate
	dOJD=dOJD-long(dOJD)+0.5;
	if (dOJD!=0.5)
	{
		//Total no of seconds, there will be 86400 seconds/day
		s=long(86400*dOJD);

		//hours
		dDate.wHour=WORD(s/3600);	
		s%=3600;
		//minutes
		dDate.wMinute=WORD(s/60);
		//seconds
		dDate.wSecond=WORD(s%60);
	}
	return dDate;
}

//Get  day of the week  0-Sun
int  DayOfTheWeek(SYSTEMTIME dDate)
{
	return long(DateToDayNumber(dDate)-1)%7;	
}

SYSTEMTIME DateAdd(DateAddOptions o,int iInterval, SYSTEMTIME dDate)
{
	switch(o)
	{
		case dd:
			return DayNumberToDate(DateToDayNumber(dDate)+iInterval);
			break;
		case mm:
			dDate.wYear+=iInterval/12;
			dDate.wMonth+=iInterval%12;
			return dDate;
			break;
		case yy:
			dDate.wYear+=iInterval;
			return dDate;
			break;
		case h:
			//convert hours into seconds then process
			iInterval=iInterval*3600;
		case m:
			//convert minutes into seconds then process
			iInterval=iInterval*60;
		case s:
			//Add No of Days involved
			dDate=DayNumberToDate(DateToDayNumber(dDate)+(iInterval/86400));
			//remaining hours
			iInterval%=86400;
			dDate.wHour+=iInterval/3600;	
			iInterval%=3600;
			//minutes
			dDate.wMinute+=iInterval/60;
			//seconds
			dDate.wSecond+=iInterval%60; 
			return dDate;
			break;
	}
	return dDate; 
}

//Get days in a  month
int  DaysInMonth(SYSTEMTIME dDate)
{
	dDate.wDay=1; 	
	return long(DateToDayNumber(DateAdd(mm,1,dDate))-DateToDayNumber(dDate));	
}

//Get First Day of the month
int  FirstOftheMonth(SYSTEMTIME dDate)
{
	dDate.wDay=1; 	
	return long(DayOfTheWeek(dDate));	
}
