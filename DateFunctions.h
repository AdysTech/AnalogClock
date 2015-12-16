enum DateAddOptions{dd=0,mm,yy,h,m,s};

//http://en.wikipedia.org/wiki/Julian_day

//Julian day (jd) from Gregorian day, month and year (d, m, y)
//returns Modified Julian Day since midnight at the beginning of Wednesday November 17, 1858
double DateToDayNumber(SYSTEMTIME dDate);

//Julian day number to the Gregorian date 
SYSTEMTIME DayNumberToDate(double lDateNumber);

//Get  day of the week  0-Sun
int  DayOfTheWeek(SYSTEMTIME dDate);

//Get days in a  month
int  DaysInMonth(SYSTEMTIME dDate);

//Get First Day of the month
int  FirstOftheMonth(SYSTEMTIME dDate);

SYSTEMTIME DateAdd(DateAddOptions o,int iInterval, SYSTEMTIME dDate);
