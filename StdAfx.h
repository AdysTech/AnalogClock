// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#endif 

#ifndef WINVER
	#define WINVER 0x0500
#endif 

#ifndef _WIN32_IE
	#define _WIN32_IE 0x0500
#endif


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//#define UNICODE

#include <windows.h>
#pragma message("Compiling pch")
#include <windowsx.h>
#include <winuser.h>
#include <ole2.h>
#include <commctrl.h>
#include <shlwapi.h>
#include <Math.h>
#include <time.h> 
#include <basetsd.h>
#pragma comment(lib, "comctl32.lib")
//#include <shlwapi.h>
//#pragma comment( lib, "shlwapi.lib" )
//#include <strsafe.h>
//#pragma comment(lib, "strsafe.lib")
#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
