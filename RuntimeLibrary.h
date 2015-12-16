// some defines for SetLayeredWindowAttributes
#ifndef LWA_COLORKEY
#define LWA_COLORKEY 0x00000001
#define LWA_ALPHA 0x00000002
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif

// and some for AnimateWindow
#ifndef AW_HOR_NEGATIVE
#define AW_HOR_POSITIVE 0x00000001
#define AW_HOR_NEGATIVE 0x00000002
#define AW_VER_POSITIVE 0x00000004
#define AW_VER_NEGATIVE 0x00000008
#define AW_CENTER 0x00000010
#define AW_HIDE 0x00010000
#define AW_ACTIVATE 0x00020000
#define AW_SLIDE 0x00040000
#define AW_BLEND 0x00080000
#endif


#if  !defined(SetLayeredWindowAttributes_)
#define SetLayeredWindowAttributes_
typedef DWORD(WINAPI *SetLayeredWindow)(HWND,COLORREF,BYTE,DWORD);
#define SetLayeredWindowAttributes(hwnd, crKey, bAlpha, dwFlags)\
    (LRESULT)(DWORD)(BOOL)(SetLayeredWindowPtr)(hwnd, crKey, bAlpha, dwFlags)
#endif

bool InitLibrary();
bool TrnsparencySupported();
