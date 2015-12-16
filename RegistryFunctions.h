
const LPCTSTR HomePath= "Software\\Ady'sTech\\AnalogClock";

int SaveSetting(LPCTSTR sBranchName,LPCTSTR sKeyName, int iDataType, BYTE& Data,unsigned long idatasize,bool bVolatile=false);
int ReadSetting(LPCTSTR sKeyName,LPCTSTR sValueName,BYTE& Data,unsigned long iDataSize);
int RemoveSetting(LPCTSTR sKeyName,LPCTSTR sValueName);
int RegDeleteTree(HKEY hKeyRoot, char* sKeytoDelete);