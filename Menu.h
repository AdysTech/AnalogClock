//ToggleMenuCheck(submenu,menu id), returns true if earlier checked
bool ToggleMenuCheckMark(HMENU hMainMenu,int MenuID);
//SetMenuState (submenu,menu id,newstate)
bool SetMenuState(HMENU hMainMenu,int MenuID,UINT iData, bool SetType=false);
bool IsCustomMenuID(HMENU hMenu, int MenuID);
