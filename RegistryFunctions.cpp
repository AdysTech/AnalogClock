#include "StdAfx.h"
#include "RegistryFunctions.h"


int SaveSetting(LPCTSTR sKeyName,LPCTSTR sValueName, int iDataType, BYTE& Data,unsigned long iDataSize,bool bVolatile)
{
	HKEY hKeyHome;int lResult;char sKeyPath[256];	
	if (sKeyName!=NULL)
	{
		strcpy_s(sKeyPath,256,HomePath);
		strcat_s(sKeyPath,256,"\\");
		strcat_s(sKeyPath,256,sKeyName);
	}
	else
		strcpy_s(sKeyPath,256,HomePath);
	
	if(bVolatile)	
		lResult=RegCreateKeyEx(HKEY_CURRENT_USER,sKeyPath,0,0,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,&hKeyHome,NULL);
	else
		lResult=RegCreateKeyEx(HKEY_CURRENT_USER,sKeyPath,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKeyHome,NULL);

	if (lResult != ERROR_SUCCESS)
	{
		PVOID sMessage;
		lResult=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,0,lResult,0,(LPTSTR)&sMessage,MAX_PATH,NULL);
		if(lResult)
			MessageBox(NULL,(LPCTSTR)sMessage,TEXT("Error Saving Settings"),MB_OK|MB_ICONSTOP);
		LocalFree(sMessage);
		return -1;
	}

	lResult=RegSetValueEx(hKeyHome,sValueName,0,iDataType,(const BYTE*)&Data,iDataSize);

	RegFlushKey(hKeyHome);
	RegCloseKey(hKeyHome);
	return lResult;
}

int ReadSetting(LPCTSTR sKeyName,LPCTSTR sValueName,BYTE& Data,unsigned long iDataSize)
{
	HKEY hKeyHome;long lResult;	char sKeyPath[256];	unsigned char *sBuffer;
	unsigned long iSize;

	if (sKeyName!=NULL)
	{
		strcpy_s(sKeyPath,256,HomePath);
		strcat_s(sKeyPath,256,"\\");
		strcat_s(sKeyPath,256,sKeyName);
	}
	else
		strcpy_s(sKeyPath,256,HomePath);
	
	lResult=RegOpenKeyEx(HKEY_CURRENT_USER,sKeyPath,0,KEY_READ,&hKeyHome);
	if (lResult != ERROR_SUCCESS)		
		return -1;

	lResult=RegQueryInfoKey(hKeyHome,NULL,NULL,0,NULL,NULL,NULL,NULL,NULL,&iSize,NULL,NULL);		
	
	sBuffer=new unsigned char[++iSize];
	
	lResult=RegQueryValueEx(hKeyHome,sValueName,0,0,sBuffer,&iSize);

	RegCloseKey(hKeyHome);
	if (lResult != ERROR_SUCCESS)		
		return -1;

	if (iSize<=iDataSize)
		memcpy(&Data,sBuffer,iSize);
	else
		memcpy(&Data,sBuffer,iDataSize);
	delete sBuffer;

	return iSize;
		
}

int RemoveSetting(LPCTSTR sKeyName,LPCTSTR sValueName)
{

	HKEY hKeyHome;long lResult;	char sKeyPath[256];

	if (sKeyName!=NULL)
	{
		strcpy_s(sKeyPath,256,HomePath);
		strcat_s(sKeyPath,256,"\\");
		strcat_s(sKeyPath,256,sKeyName);
	}
	else
		strcpy_s(sKeyPath,256,HomePath);


	//If no value name specified then delete the key itself
	if(sValueName==NULL)
	{
		char *sKeyToDelete,*sPath;
		for (int i=strlen(sKeyPath);i>=0;i--)
		{
			if (sKeyPath[i]=='\\')
			{
				sPath=new char[i+1];				
				strncpy_s(sPath,i+1,sKeyPath,i);
				sPath[i]='\0';					
				//sKeyToDelete=new char[strlen(sKeyPath)-i];
				sKeyToDelete=new char[MAX_PATH];
				strcpy_s(sKeyToDelete,MAX_PATH,sKeyPath+(i+1));
				break;
			}
		}
		lResult=RegOpenKeyEx(HKEY_CURRENT_USER,sPath,0,KEY_ALL_ACCESS,&hKeyHome);
		if (!hKeyHome) return -1;
		RegDeleteTree(hKeyHome,sKeyToDelete);
		RegCloseKey(hKeyHome);
		delete sPath;
		delete sKeyToDelete;
		return 1;
	}
	else
	{
		lResult=RegOpenKeyEx(HKEY_CURRENT_USER,sKeyPath,0,KEY_ALL_ACCESS,&hKeyHome);
		if (!hKeyHome) return -1;
		return RegDeleteValue(hKeyHome,sValueName);
	}


}

int RegDeleteTree(HKEY hKeyRoot,char* sKeytoDelete)
{
	HKEY hKey;unsigned long lResult, iKeyCount=0,iKeyLen=256,iKeyNum; 
	char* sKeyName;char *sBranch; int iBranchLen=0;
		
	// First, see if we can delete the key without having to recurse.
	lResult=RegDeleteKey(hKeyRoot,sKeytoDelete);
	if (lResult==ERROR_SUCCESS)
		return 1;

	//delete all sub keys recursively
	lResult=RegOpenKeyEx(hKeyRoot,sKeytoDelete,0, KEY_READ, &hKey);
	if (lResult != ERROR_SUCCESS) 
	{
		if (lResult == ERROR_FILE_NOT_FOUND)
			return 1;
		else
			return -1;
	}

	iKeyLen=MAX_PATH;
	lResult=RegQueryInfoKey(hKey,NULL,NULL,0,&iKeyCount,&iKeyLen,NULL,NULL,NULL,NULL,NULL,NULL);		
	sKeyName=new char[++iKeyLen];
	iBranchLen=strlen(sKeytoDelete)+iKeyLen+1;
	sBranch=new char[iBranchLen];

	for (iKeyNum=0;iKeyNum<iKeyCount;iKeyNum++)
	{
		lResult=RegEnumKeyEx(hKey,0,sKeyName,&iKeyLen,0,NULL,NULL,NULL);
		if (lResult == ERROR_SUCCESS)
		{
			strcpy_s(sBranch,iBranchLen,sKeytoDelete);
			strcat_s(sBranch,iBranchLen,"\\");			
			strcat_s(sBranch,iBranchLen, sKeyName);
			if (RegDeleteTree(hKeyRoot, sBranch)!=1)
                break;
		}
	}			

	delete sBranch;
	delete sKeyName;
	RegCloseKey (hKey);

   // Try again to delete the key.
    lResult = RegDeleteKey(hKeyRoot, sKeytoDelete);

    if (lResult == ERROR_SUCCESS) 
        return 1;

    return -1;
}

