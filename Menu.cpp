#include "StdAfx.h"
#include "Menu.h"

bool ToggleMenuCheckMark(HMENU hMainMenu,int MenuID)
{
	HMENU hMenu; UINT iState;
	hMenu=GetSubMenu(hMainMenu, 0);
	if (!hMenu) hMenu=hMainMenu;
	iState=GetMenuState(hMenu,MenuID,MF_BYCOMMAND);
	if (iState==MF_CHECKED||iState%MFT_RADIOCHECK==MF_CHECKED)
		//CheckMenuItem(hMenu,MenuID,MF_BYCOMMAND|MF_UNCHECKED);
		SetMenuState(hMenu,MenuID,MFS_UNCHECKED);
	else
		//CheckMenuItem(hMenu,MenuID,MF_BYCOMMAND|MF_CHECKED);
		SetMenuState(hMenu,MenuID,MFS_CHECKED);

	return (iState==MF_CHECKED);
}

bool SetMenuState(HMENU hMainMenu,int MenuID,UINT iData, bool SetType)
{
	MENUITEMINFO miInfo;
	miInfo.cbSize =sizeof(MENUITEMINFO);
	
	if (SetType)
	{
		miInfo.fType=iData;
		miInfo.fMask=MIIM_FTYPE;
	}
	else
	{
		miInfo.fState=iData;	
		miInfo.fMask=MIIM_STATE;
	}

	if (SetMenuItemInfo(hMainMenu,MenuID,false,&miInfo))
		return true;
	else
		return false;


}

bool IsCustomMenuID(HMENU hMenu, int MenuID)
{
	MENUITEMINFO miInfo;
	miInfo.cbSize =sizeof(MENUITEMINFO);
	miInfo.fMask=MIIM_STATE;
	if(GetMenuItemInfo(hMenu,MenuID,false,&miInfo)!=0)
		return true;
	else
		return false;
}