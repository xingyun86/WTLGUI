// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0500

#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#define _CSTRING_NS
#define _WTYPES_NS

#include <atlstr.h>
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlddx.h>
#include <atlcrack.h>
#include <atlctrlw.h>
#include <atlmisc.h>
#include <atltypes.h>

#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>
#include <time.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

#include <string>
#include <map>
#include <vector>

#if !defined(_UNICODE) && !defined(UNICODE)
#define _TSTRING	std::string
#else
#define _TSTRING	std::wstring
#endif //!defined(_UNICODE) && !defined(UNICODE)

#define TSTRING _TSTRING
#define tstring _TSTRING

__inline static tstring AToT(std::string str);

__inline static tstring WToT(std::wstring wstr);

__inline static VOID GetNativeSystemInformation(SYSTEM_INFO & system_info)
{
	typedef void (WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO);

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
	LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandle(_T("KERNEL32.DLL")), "GetNativeSystemInfo");
	if (fnGetNativeSystemInfo)
	{
		fnGetNativeSystemInfo(&system_info);
	}
	else
	{
		GetSystemInfo(&system_info);
	}
}

//��ȡ����ʱ�������(���ؽ��Ϊ100�����ʱ��, 1ns=1 000 000ms=1000 000 000s)
CONST ULONGLONG MILLI_100NANO = 1000000ULL / 100ULL;
__inline static LONGLONG GetCurrentMillisecons()
{
	FILETIME ft = { 0 };
	SYSTEMTIME st = { 0 };
	ULARGE_INTEGER u = { 0, 0 };
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	u.HighPart = ft.dwHighDateTime;
	u.LowPart = ft.dwLowDateTime;
	return u.QuadPart;
}

namespace NearSideAutoHide{

#define NEAR_SIZE 1 //�����Զ�ͣ����Ч����
#define NEAR_SIDE 1 //�������غ�����Ļ�ϱ��������أ���ʹ�����Դ���

#define IDC_TIMER_NEARSIDEHIDE	0xFFFF
#define T_TIMEOUT_NEARSIDEHIDE	0xFF
	enum {
		ALIGN_NONE,          //��ͣ��
		ALIGN_TOP,          //ͣ���ϱ�
		ALIGN_LEFT,          //ͣ�����
		ALIGN_RIGHT          //ͣ���ұ�
	};
	static int g_nScreenX = 0;
	static int g_nScreenY = 0;
	static int g_nAlignType = ALIGN_NONE;   //ȫ�ֱ��������ڼ�¼����ͣ��״̬
	
	__inline static void InitScreenSize()
	{
		g_nScreenX = GetSystemMetrics(SM_CXSCREEN);
		g_nScreenY = GetSystemMetrics(SM_CYSCREEN);
	}
	//�ڴ����ʼ�����趨����״̬���������ͣ������ͣ���ڱ�Ե
	//�ұ���Ѱ�����������������ʼ����������Ϊ��רһѰ��һ��������
	//���ǣ������ʼ��ʱ������WM_MOVING��Ϣ,�Ҳ��ò��ظ���������.
	__inline static void NearSide(HWND hWnd, LPRECT lpRect)
	{
		LONG Width = lpRect->right - lpRect->left;
		LONG Height = lpRect->bottom - lpRect->top;
		BOOL bChange = 0;
		g_nAlignType = ALIGN_NONE;
		if (lpRect->left < NEAR_SIZE)
		{
			g_nAlignType = ALIGN_LEFT;
			if ((lpRect->left != 0) && lpRect->right != NEAR_SIDE)
			{
				lpRect->left = 0;
				lpRect->right = Width;
				bChange = FALSE;
			}
		}
		else if (lpRect->right > g_nScreenX - NEAR_SIZE)
		{
			g_nAlignType = ALIGN_RIGHT;
			if (lpRect->right != g_nScreenX && lpRect->left != g_nScreenX - NEAR_SIDE)
			{
				lpRect->right = g_nScreenX;
				lpRect->left = g_nScreenX - Width;
				bChange = FALSE;
			}
		}
		//������
		else if (lpRect->top < NEAR_SIZE)
		{
			g_nAlignType = ALIGN_TOP;
			if (lpRect->top != 0 && lpRect->bottom != NEAR_SIDE)
			{
				lpRect->top = 0;
				lpRect->bottom = Height;
				bChange = FALSE;
			}
		}
		if (bChange)
		{
			::MoveWindow(hWnd, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bChange);
		}
	}
	
	//�������ʾ�����ɸú������,����bHide������ʾ��������.
	__inline static void AutoHideProc(HWND hWnd, LPRECT lpRect, BOOL bHide)
	{
		int nStep = 20;  //������������Ĳ���,�������ò���ƽ��,���������ֵ.
		int xStep = 0, xEnd = 0;
		int yStep = 0, yEnd = 0;
		LONG Width = lpRect->right - lpRect->left;
		LONG Height = lpRect->bottom - lpRect->top;

		//�±��жϴ��������ƶ�,��ͣ����ʽ����
		switch (g_nAlignType)
		{
		case ALIGN_TOP:
		{
			//�����Ʋ�
			xStep = 0;
			xEnd = lpRect->left;
			if (bHide)
			{
				yStep = -lpRect->bottom / nStep;
				yEnd = -Height + NEAR_SIDE;
			}
			else
			{
				yStep = -lpRect->top / nStep;
				yEnd = 0;
			}
			break;
		}
		case ALIGN_LEFT:
		{
			//�����Ʋ�
			yStep = 0;
			yEnd = lpRect->top;
			if (bHide)
			{
				xStep = -lpRect->right / nStep;
				xEnd = -Width + NEAR_SIDE;
			}
			else
			{
				xStep = -lpRect->left / nStep;
				xEnd = 0;
			}
			break;
		}
		case ALIGN_RIGHT:
		{
			//�����Ʋ�
			yStep = 0;
			yEnd = lpRect->top;
			if (bHide)
			{
				xStep = (g_nScreenX - lpRect->left) / nStep;
				xEnd = g_nScreenX - NEAR_SIDE;
			}
			else
			{
				xStep = (g_nScreenX - lpRect->right) / nStep;
				xEnd = g_nScreenX - Width;
			}
			break;
		}
		default:
			return;
		}
		//������������.
		for (int i = 0; i < nStep; i++)
		{
			lpRect->left += xStep;
			lpRect->top += yStep;
			::SetWindowPos(hWnd, NULL, lpRect->left, lpRect->top, 0, 0, SWP_NOSIZE | SWP_NOSENDCHANGING);
			::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			Sleep(3);
		}
		::SetWindowPos(hWnd, NULL, xEnd, yEnd, 0, 0, SWP_NOSIZE | SWP_NOSENDCHANGING);
		if (!bHide) //��������ѱ���ʾ,���ö�ʱ��.�������.
		{
			::SetTimer(hWnd, WM_TIMER, WAIT_TIMEOUT, NULL);
		}
	}

	// WM_TIMER
	__inline static LRESULT OnTimer(HWND hWnd, UINT uTimerID)
	{
		LRESULT lResult = FALSE;
		switch (uTimerID)
		{
		case IDC_TIMER_NEARSIDEHIDE:
		{
			RECT rc = { 0 };
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			::GetWindowRect(hWnd, &rc);

			if (!PtInRect(&rc, pt)) //����겻�ڴ�����,���ش���.
			{
				::KillTimer(hWnd, uTimerID);
				AutoHideProc(hWnd, &rc, TRUE);
			}
			lResult = TRUE;
		}
		break;
		default:
		{
			//no-op
		}
		break;
		}
		return lResult;
	}
	// WM_NCMOUSEMOVE
	__inline static LRESULT OnNcMouseMove(HWND hWnd)
	{
		RECT rc = { 0 };
		::GetWindowRect(hWnd, &rc);
		if (rc.left < 0 || rc.top < 0 || rc.right > g_nScreenX) //δ��ʾ
		{
			AutoHideProc(hWnd, &rc, FALSE);
		}
		else
		{
			::SetTimer(hWnd, IDC_TIMER_NEARSIDEHIDE, T_TIMEOUT_NEARSIDEHIDE, NULL);
		}
		return 0;
	}
	// WM_MOUSEMOVE
	__inline static LRESULT OnMouseMove(HWND hWnd)
	{
		RECT rc = { 0 };
		::GetWindowRect(hWnd, &rc);
		if (rc.left < 0 || rc.top < 0 || rc.right > g_nScreenX) //δ��ʾ
		{
			AutoHideProc(hWnd, &rc, FALSE);
		}
		else
		{
			::SetTimer(hWnd, IDC_TIMER_NEARSIDEHIDE, T_TIMEOUT_NEARSIDEHIDE, NULL);
		}
		return 0;
	}
	// WM_ENTERSIZEMOVE
	__inline static LRESULT OnEnterSizeMove(HWND hWnd)
	{
		::KillTimer(hWnd, IDC_TIMER_NEARSIDEHIDE);
		return 0;
	}
	// WM_EXITSIZEMOVE
	__inline static LRESULT OnExitSizeMove(HWND hWnd)
	{
		::SetTimer(hWnd, IDC_TIMER_NEARSIDEHIDE, T_TIMEOUT_NEARSIDEHIDE, NULL);
		return 0;
	}
	/////////////////////////////////////////////////////////////
	// WM_MOVING
	//������봦������ϢWM_MOVING��lParam�ǲ���RECTָ��
	__inline static LRESULT OnMoving(HWND hWnd, LPARAM lParam)
	{
		POINT pt = { 0 };
		LPRECT lpRect = (LPRECT)lParam;
		LONG Width = lpRect->right - lpRect->left;
		LONG Height = lpRect->bottom - lpRect->top;

		//δ���߽���pRect����
		if (g_nAlignType == ALIGN_NONE)
		{
			if (lpRect->left < NEAR_SIZE) //�������Ч������
			{
				g_nAlignType = ALIGN_LEFT;
				lpRect->left = 0;
				lpRect->right = Width;
			}
			if (lpRect->right + NEAR_SIZE > g_nScreenX) //���ұ���Ч�����ڣ�g_nScreenXΪ��Ļ��ȣ�����GetSystemMetrics(SM_CYSCREEN)�õ���
			{
				g_nAlignType = ALIGN_RIGHT;
				lpRect->right = g_nScreenX;
				lpRect->left = g_nScreenX - Width;
			}
			if (lpRect->top < NEAR_SIZE) //���ϱ���Ч�����ڣ��Զ���£��
			{
				g_nAlignType = ALIGN_TOP;
				lpRect->top = 0;
				lpRect->bottom = Height;
			}
		}
		else
		{
			//���߽���������
			::GetCursorPos(&pt);
			if (g_nAlignType == ALIGN_TOP)
			{
				lpRect->top = 0;
				lpRect->bottom = Height;
				if (pt.y > NEAR_SIZE) //������뿪�ϱ߽����ϲ�ͣ����
				{
					g_nAlignType = ALIGN_NONE;
				}
				else
				{
					if (lpRect->left < NEAR_SIZE) //���ϲ�ͣ��ʱ������Ҳ�������ұ߽ǡ�
					{
						lpRect->left = 0;
						lpRect->right = Width;
					}
					else if (lpRect->right + NEAR_SIZE > g_nScreenX)
					{
						lpRect->right = g_nScreenX;
						lpRect->left = g_nScreenX - Width;
					}
				}
			}
			if (g_nAlignType == ALIGN_LEFT)
			{
				lpRect->left = 0;
				lpRect->right = Width;
				if (pt.x > NEAR_SIZE) //���������뿪��߽�ʱ���ͣ����
				{
					g_nAlignType = ALIGN_NONE;
				}
				else
				{
					if (lpRect->top < NEAR_SIZE) //�������Ͻǡ�
					{
						lpRect->top = 0;
						lpRect->bottom = Height;
					}
				}
			}
			else if (g_nAlignType == ALIGN_RIGHT)
			{
				lpRect->left = g_nScreenX - Width;
				lpRect->right = g_nScreenX;
				if (pt.x < g_nScreenX - NEAR_SIZE) //������뿪�ұ߽�ʱ�����ͣ����
				{
					g_nAlignType = ALIGN_NONE;
				}
				else
				{
					if (lpRect->top < NEAR_SIZE) //�������Ͻǡ�
					{
						lpRect->top = 0;
						lpRect->bottom = Height;
					}
				}
			}
		}
		return 0;
	}
}
__inline static 
void NotifyUpdate(HWND hWnd, RECT & rc, BOOL bErase = TRUE)
{
	RECT rcWnd = { 0 };

	::GetClientRect(hWnd, &rcWnd);
	if (memcmp(&rc, &rcWnd, sizeof(rc)))
	{
		::InvalidateRect(hWnd, &rcWnd, bErase);
		memcpy(&rc, &rcWnd, sizeof(rc));
	}
}

__inline static
bool SaveBitmapToFile(HDC hDC, HBITMAP hBitmap, const _TCHAR * ptFileName)
{
//	HDC hDC;
	//�豸������
	int iBits;
	//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD wBitCount;
	//λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С ��  λͼ�ļ���С �� д���ļ��ֽ���
	DWORD  dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;
	BITMAP  Bitmap;
	//λͼ���Խṹ
	BITMAPFILEHEADER   bmfHdr;
	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER   bi;
	//λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;
	//ָ��λͼ��Ϣͷ�ṹ
	HANDLE  fh, hDib, hPal;
	HPALETTE  hOldPal = NULL;

	//�����ļ��������ڴ�������ɫ����
	//����λͼ�ļ�ÿ��������ռ�ֽ���
	//hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	//DeleteDC(hDC);
	if (iBits <= 1)
	{
		wBitCount = 1;
	}
	else if (iBits <= 4)
	{
		wBitCount = 4;
	}
	else if (iBits <= 8)
	{
		wBitCount = 8;
	}
	else if (iBits <= 24)
	{
		wBitCount = 24;
	}
	else
	{
		wBitCount = 32;
	}

	//�����ɫ���С
	if (wBitCount <= 8)
	{
		dwPaletteSize = (1 << wBitCount)*sizeof(RGBQUAD);
	}

	//����λͼ��Ϣͷ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ�
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// �����ɫ��
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
	//�ָ���ɫ��   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//����λͼ�ļ�    
	fh = CreateFile(ptFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		GlobalUnlock(hDib);
		GlobalFree(hDib);
		return FALSE;
	}
	//����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize, &dwWritten, NULL);

	GlobalUnlock(hDib);
	GlobalFree(hDib);

	CloseHandle(fh);
	return false;
}
__inline static
bool SaveBitmapToFile(HBITMAP hBitmap, const _TCHAR * ptFileName)
{
	HDC hDC;
	//�豸������
	int iBits;
	//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD wBitCount;
	//λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С ��  λͼ�ļ���С �� д���ļ��ֽ���
	DWORD  dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;
	BITMAP  Bitmap;
	//λͼ���Խṹ
	BITMAPFILEHEADER   bmfHdr;
	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER   bi;
	//λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;
	//ָ��λͼ��Ϣͷ�ṹ
	HANDLE  fh, hDib, hPal;
	HPALETTE  hOldPal = NULL;

	//�����ļ��������ڴ�������ɫ����
	//����λͼ�ļ�ÿ��������ռ�ֽ���
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
	{
		wBitCount = 1;
	}
	else if (iBits <= 4)
	{
		wBitCount = 4;
	}
	else if (iBits <= 8)
	{
		wBitCount = 8;
	}
	else if (iBits <= 24)
	{
		wBitCount = 24;
	}
	else
	{
		wBitCount = 32;
	}

	//�����ɫ���С
	if (wBitCount <= 8)
	{
		dwPaletteSize = (1 << wBitCount)*sizeof(RGBQUAD);
	}

	//����λͼ��Ϣͷ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ�
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// �����ɫ��
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
	//�ָ���ɫ��   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//����λͼ�ļ�    
	fh = CreateFile(ptFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		GlobalUnlock(hDib);
		GlobalFree(hDib);
		return FALSE;
	}
	//����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize, &dwWritten, NULL);

	GlobalUnlock(hDib);
	GlobalFree(hDib);

	CloseHandle(fh);
	return false;
}
__inline static 
HBITMAP GetBitmapFromDC(HDC hDC, RECT rc, RECT rcMemory = { 0 })
{
	HDC hMemoryDC = NULL;
	HBITMAP hBitmap = NULL;
	HBITMAP hBitmapTemp = NULL;

	//�����豸������(HDC)
	hMemoryDC = CreateCompatibleDC(hDC);
	
	//����HBITMAP
	hBitmap = CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
	hBitmapTemp = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
	
	if (rcMemory.right <= rcMemory.left)
	{
		rcMemory.right = rcMemory.left + rc.right - rc.left;
	}
	if (rcMemory.bottom <= rcMemory.top)
	{
		rcMemory.bottom = rcMemory.top + rc.bottom - rc.top;
	}

	//�õ�λͼ������
	StretchBlt(hMemoryDC, rcMemory.left, rcMemory.top, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top,
		hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SRCCOPY);
	
	//�õ����յ�λͼ��Ϣ
	hBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmapTemp);

	//�ͷ��ڴ�
	DeleteObject(hBitmapTemp);
	hBitmapTemp = NULL;
	::DeleteDC(hMemoryDC);
	hMemoryDC = NULL;

	return hBitmap;
}

__inline static 
void DrawMemoryBitmap(HDC &dc, HWND hWnd, LONG lWidth, LONG lHeight, HBITMAP hBitmap)
{
	RECT rect;
	HBITMAP hOldBitmap;
	int disHeight, disWidth;

	GetClientRect(hWnd, &rect);//��ȡ�ͻ�����С
	disHeight = rect.bottom - rect.top;
	disWidth = rect.right - rect.left;

	HDC mDc = ::CreateCompatibleDC(dc);//������ǰ�����ĵļ���dc(�ڴ�DC)
	hOldBitmap = (HBITMAP)::SelectObject(mDc, hBitmap);//��λͼ���ص��ڴ�DC

	//�����ڴ�DC���ݿ鵽��ǰDC���Զ�����
	::StretchBlt(dc, 0, 0, disWidth, disHeight, mDc, 0, 0, lWidth, lHeight, SRCCOPY);

	//�ָ��ڴ�ԭʼ����
	::SelectObject(mDc, hOldBitmap);

	//ɾ����Դ����ֹй©
	::DeleteObject(hBitmap);
	::DeleteDC(mDc);
}

__inline static
void DrawImage(HDC &dc, HWND hWnd, LONG lWidth, LONG lHeight, const _TCHAR * ptFileName)
{
	RECT rect;
	HBITMAP hOrgBitmap;
	HBITMAP hOldBitmap;
	int disHeight, disWidth;

	GetClientRect(hWnd, &rect);//��ȡ�ͻ�����С
	disHeight = rect.bottom - rect.top;
	disWidth = rect.right - rect.left;

	//����ͼƬ
	hOrgBitmap = (HBITMAP)::LoadImage(GetModuleHandle(NULL), ptFileName, IMAGE_BITMAP, lWidth, lHeight, LR_LOADFROMFILE);

	HDC mDc = ::CreateCompatibleDC(dc);//������ǰ�����ĵļ���dc(�ڴ�DC)
	hOldBitmap = (HBITMAP)::SelectObject(mDc, hOrgBitmap);//��λͼ���ص��ڴ�DC

	//�����ڴ�DC���ݿ鵽��ǰDC���Զ�����
	::StretchBlt(dc, 0, 0, disWidth, disHeight, mDc, 0, 0, lWidth, lHeight, SRCCOPY);

	//�ָ��ڴ�ԭʼ����
	::SelectObject(mDc, hOldBitmap);

	//ɾ����Դ����ֹй©
	::DeleteObject(hOrgBitmap);
	::DeleteDC(mDc);
	mDc = NULL;
}

__inline static 
HBITMAP DrawAlphaBlend(HWND hWnd, HDC hDCWnd)
{
	typedef struct _ALPHABLENDRECT {	
		HDC HDCDST;
		RECT RCDST;
		HDC HDCSRC;
		RECT RCSRC;
		BLENDFUNCTION BF;// structure for alpha blending 
	}ALPHABLENDRECT, *PALPHABLENDRECT;
	UINT32 x = 0;// stepping variables 
	UINT32 y = 0;// stepping variables 
	HDC hDC = NULL;// handle of the DC we will create 
	UCHAR uA = 0x00;// used for doing transparent gradient 
	UCHAR uR = 0x00;
	UCHAR uG = 0x00;
	UCHAR uB = 0x00;
	float fAF = 0.0f;// used to do premultiply
	VOID * pvBits = 0;// pointer to DIB section
	RECT rcWnd = { 0 };// used for getting window dimensions
	HBITMAP hBitmap = NULL;// bitmap handle
	BITMAPINFO bmi = { 0 };// bitmap header
	ULONG ulWindowWidth = 0;// window width/height
	ULONG ulBitmapWidth = 0;// bitmap width/height
	ULONG ulWindowHeight = 0;// window width/height
	ULONG ulBitmapHeight = 0;// bitmap width/height
	ALPHABLENDRECT abrc = { 0 };

	// get dest dc
	abrc.HDCDST = hDCWnd;

	// get window dimensions 
	::GetClientRect(hWnd, &rcWnd);

	// calculate window width/height 
	ulWindowWidth = rcWnd.right - rcWnd.left;
	ulWindowHeight = rcWnd.bottom - rcWnd.top;

	// make sure we have at least some window size 
	if ((!ulWindowWidth) || (!ulWindowHeight))
	{
		return NULL;
	}

	// divide the window into 3 horizontal areas 
	ulWindowHeight = ulWindowHeight / 3;

	// create a DC for our bitmap -- the source DC for AlphaBlend  
	abrc.HDCSRC = ::CreateCompatibleDC(abrc.HDCDST);

	// zero the memory for the bitmap info 
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));

	// setup bitmap info  
	// set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas. 
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = ulBitmapWidth = ulWindowWidth;// -(ulWindowWidth / 5) * 2;
	bmi.bmiHeader.biHeight = ulBitmapHeight = ulWindowHeight - (ulWindowHeight / 5) * 2;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;// four 8-bit components 
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

	// create our DIB section and select the bitmap into the dc 
	hBitmap = ::CreateDIBSection(abrc.HDCSRC, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x00);
	::SelectObject(abrc.HDCSRC, hBitmap);

	// in top window area, constant alpha = 50%, but no source alpha 
	// the color format for each pixel is 0xaarrggbb  
	// set all pixels to blue and set source alpha to zero 
	for (y = 0; y < ulBitmapHeight; y++)
	{
		for (x = 0; x < ulBitmapWidth; x++)
		{
			((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x0000FF00;// 0x000000FF;
		}
	}

	abrc.RCDST.left = 0;// ulWindowWidth / 5;
	abrc.RCDST.top = ulWindowHeight / 5;
	abrc.RCDST.right = ulBitmapWidth + abrc.RCDST.left;
	abrc.RCDST.bottom = ulBitmapHeight + abrc.RCDST.top;

	abrc.RCSRC.left = 0;
	abrc.RCSRC.top = 0;
	abrc.RCSRC.right = ulBitmapWidth + abrc.RCSRC.left;
	abrc.RCSRC.bottom = ulBitmapHeight + abrc.RCSRC.top;

	abrc.BF.BlendOp = AC_SRC_OVER;
	abrc.BF.BlendFlags = 0;
	abrc.BF.AlphaFormat = 0;// ignore source alpha channel 
	abrc.BF.SourceConstantAlpha = 0x7F;// half of 0x7F = 50% transparency 
	
	if (!AlphaBlend(abrc.HDCDST,
		abrc.RCDST.left, abrc.RCDST.top,
		abrc.RCDST.right - abrc.RCDST.left,
		abrc.RCDST.bottom - abrc.RCDST.top,
		abrc.HDCSRC,
		abrc.RCSRC.left, abrc.RCSRC.top,
		abrc.RCSRC.right - abrc.RCSRC.left,
		abrc.RCSRC.bottom - abrc.RCSRC.top,
		abrc.BF))
	{
		return NULL;// alpha blend failed 
	}
	// in middle window area, constant alpha = 100% (disabled), source  
	// alpha is 0 in middle of bitmap and opaque in rest of bitmap  
	for (y = 0; y < ulBitmapHeight; y++)
	{
		for (x = 0; x < ulBitmapWidth; x++)
		{
			if ((x > (int)(ulBitmapWidth / 5)) && (x < (ulBitmapWidth - ulBitmapWidth / 5)) &&
				(y > (int)(ulBitmapHeight / 5)) && (y < (ulBitmapHeight - ulBitmapHeight / 5)))
			{
				//in middle of bitmap: source alpha = 0 (transparent). 
				// This means multiply each color component by 0x00. 
				// Thus, after AlphaBlend, we have A, 0x00 * R,  
				// 0x00 * G,and 0x00 * B (which is 0x00000000) 
				// for now, set all pixels to red 
				((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x00FF0000;
			}
			else
			{
				// in the rest of bitmap, source alpha = 0xFF (opaque)  
				// and set all pixels to blue  
				((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0xFF00FF00;// 0xFF0000FF;
			}
		}
	}

	abrc.RCDST.left = 0;// ulWindowWidth / 5;
	abrc.RCDST.top = ulWindowHeight / 5 + ulWindowHeight;
	abrc.RCDST.right = ulBitmapWidth + abrc.RCDST.left;
	abrc.RCDST.bottom = ulBitmapHeight + abrc.RCDST.top;

	abrc.RCSRC.left = 0;
	abrc.RCSRC.top = 0;
	abrc.RCSRC.right = ulBitmapWidth + abrc.RCSRC.left;
	abrc.RCSRC.bottom = ulBitmapHeight + abrc.RCSRC.top;

	abrc.BF.BlendOp = AC_SRC_OVER;
	abrc.BF.BlendFlags = 0;
	abrc.BF.AlphaFormat = AC_SRC_ALPHA;// ignore source alpha channel 
	abrc.BF.SourceConstantAlpha = 0xFF;// half of 0xFF = 50% transparency 
	
	if (!AlphaBlend(abrc.HDCDST,
		abrc.RCDST.left, abrc.RCDST.top,
		abrc.RCDST.right - abrc.RCDST.left,
		abrc.RCDST.bottom - abrc.RCDST.top,
		abrc.HDCSRC,
		abrc.RCSRC.left, abrc.RCSRC.top,
		abrc.RCSRC.right - abrc.RCSRC.left,
		abrc.RCSRC.bottom - abrc.RCSRC.top,
		abrc.BF))
	{
		return NULL;// alpha blend failed 
	}

	// bottom window area, use constant alpha = 75% and a changing 
	// source alpha. Create a gradient effect using source alpha, and  
	// then fade it even more with constant alpha 
	uR = 0x00;
	uG = 0xFF;// 0x00;
	uB = 0x00;// 0xFF;

	for (y = 0; y < ulBitmapHeight; y++)
	{
		for (x = 0; x < ulBitmapWidth; x++)
		{
			// for a simple gradient, base the alpha value on the x  
			// value of the pixel  
			uA = (UCHAR)((float)x / (float)ulBitmapWidth * 0xFF);
			//calculate the factor by which we multiply each component 
			fAF = (float)uA / (float)0xFF;
			// multiply each pixel by fAlphaFactor, so each component  
			// is less than or equal to the alpha value. 
			((UINT32 *)pvBits)[x + y * ulBitmapWidth] =
				((UCHAR)(uA * 0x1) << 0x18)| //0xAA000000 
				((UCHAR)(uR * fAF) << 0x10)| //0x00RR0000 
				((UCHAR)(uG * fAF) << 0x08)| //0x0000GG00 
				((UCHAR)(uB * fAF) << 0x00); //0x000000BB 
		}
	}

	abrc.RCDST.left = 0;// ulWindowWidth / 5;
	abrc.RCDST.top = ulWindowHeight / 5 + 2 * ulWindowHeight;
	abrc.RCDST.right = ulBitmapWidth + abrc.RCDST.left;
	abrc.RCDST.bottom = ulBitmapHeight + abrc.RCDST.top;

	abrc.RCSRC.left = 0;
	abrc.RCSRC.top = 0;
	abrc.RCSRC.right = ulBitmapWidth + abrc.RCSRC.left;
	abrc.RCSRC.bottom = ulBitmapHeight + abrc.RCSRC.top;

	abrc.BF.BlendOp = AC_SRC_OVER;
	abrc.BF.BlendFlags = 0;
	abrc.BF.AlphaFormat = AC_SRC_ALPHA;// ignore source alpha channel 
	abrc.BF.SourceConstantAlpha = 0xBF;// use constant alpha, with 75% opaqueness  
	
	if (!AlphaBlend(abrc.HDCDST,
		abrc.RCDST.left, abrc.RCDST.top,
		abrc.RCDST.right - abrc.RCDST.left,
		abrc.RCDST.bottom - abrc.RCDST.top,
		abrc.HDCSRC,
		abrc.RCSRC.left, abrc.RCSRC.top,
		abrc.RCSRC.right - abrc.RCSRC.left,
		abrc.RCSRC.bottom - abrc.RCSRC.top,
		abrc.BF))
	{
		return NULL;// alpha blend failed 
	}
	
	// do cleanup 
	DeleteObject(hBitmap);
	DeleteDC(hDC);
	hDC = NULL;

	return hBitmap;
}

#define ARGB(uA,uR,uG,uB) ((UCHAR)(uA) << 0x18) | ((UCHAR)(uR) << 0x10) | ((UCHAR)(uG) << 0x08) | ((UCHAR)(uB) << 0x00)

__inline static
HBITMAP DrawAlphaBlendRect(HWND hWnd, ULONG (&uARGB)[2], HDC hDCWnd, RECT * pRect)
{
	typedef struct _ALPHABLENDRECT {	
		HDC HDCDST;
		RECT RCDST;
		HDC HDCSRC;
		RECT RCSRC;
		BLENDFUNCTION BF;// structure for alpha blending 
	}ALPHABLENDRECT, *PALPHABLENDRECT;
	UINT32 x = 0;// stepping variables
	UINT32 y = 0;// stepping variables 
	UCHAR uA = 0x00;// used for doing transparent gradient 
	UCHAR uR = 0x00;
	UCHAR uG = 0x00;
	UCHAR uB = 0x00;
	float fAF = 0.0f;// used to do premultiply
	VOID * pvBits = 0;// pointer to DIB section
	RECT rcWnd = { 0 };// used for getting window dimensions
	HBITMAP hBitmap = NULL;// bitmap handle
	BITMAPINFO bmi = { 0 };// bitmap header
	ULONG ulWindowWidth = 0;// window width/height
	ULONG ulBitmapWidth = 0;// bitmap width/height
	ULONG ulWindowHeight = 0;// window width/height
	ULONG ulBitmapHeight = 0;// bitmap width/height
	ALPHABLENDRECT abrc = { 0 };

	// get dest dc
	abrc.HDCDST = hDCWnd;

	// get window dimensions 
	::GetClientRect(hWnd, &rcWnd);

	// calculate window width/height 
	ulWindowWidth = rcWnd.right - rcWnd.left;
	ulWindowHeight = rcWnd.bottom - rcWnd.top;

	// make sure we have at least some window size 
	if ((!ulWindowWidth) || (!ulWindowHeight))
	{
		return NULL;
	}

	// create a DC for our bitmap -- the source DC for AlphaBlend  
	abrc.HDCSRC = ::CreateCompatibleDC(abrc.HDCDST);

	// zero the memory for the bitmap info 
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));

	// setup bitmap info  
	// set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas. 
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = ulBitmapWidth = ulWindowWidth;
	bmi.bmiHeader.biHeight = ulBitmapHeight = ulWindowHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;// four 8-bit components 
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

	// create our DIB section and select the bitmap into the dc 
	hBitmap = ::CreateDIBSection(abrc.HDCSRC, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x00);
	::SelectObject(abrc.HDCSRC, hBitmap);
	
	// in middle window area, constant alpha = 100% (disabled), source  
	// alpha is 0 in middle of bitmap and opaque in rest of bitmap  
	for (x = 0; x < ulBitmapWidth; x++)
	{
		for (y = 0; y < ulBitmapHeight; y++)
		{
			if ((x > (int)(pRect->right)) && (x < (ulBitmapWidth - pRect->left - 1)) &&
				(y >(int)(pRect->bottom)) && (y < (ulBitmapHeight - pRect->top - 1)))
			{
				//in middle of bitmap: source alpha = 0 (transparent). 
				// This means multiply each color component by 0x00. 
				// Thus, after AlphaBlend, we have A, 0x00 * R,  
				// 0x00 * G,and 0x00 * B (which is 0x00000000) 
				// for now, set all pixels to red 
				((UINT32 *)pvBits)[x + y * ulBitmapWidth] = uARGB[0];//0x7F0000FF;// 0x00FF0000;
			}
			else
			{
				// in the rest of bitmap, source alpha = 0xFF (opaque)  
				// and set all pixels to blue  
				((UINT32 *)pvBits)[x + y * ulBitmapWidth] = uARGB[1];//0x7F00FF00;// 0xFF0000FF;
			}
		}
	}

	abrc.RCDST.left = 0;
	abrc.RCDST.top = 0;
	abrc.RCDST.right = ulBitmapWidth + abrc.RCDST.left;
	abrc.RCDST.bottom = ulBitmapHeight + abrc.RCDST.top;

	abrc.RCSRC.left = 0;
	abrc.RCSRC.top = 0;
	abrc.RCSRC.right = ulBitmapWidth + abrc.RCSRC.left;
	abrc.RCSRC.bottom = ulBitmapHeight + abrc.RCSRC.top;

	abrc.BF.BlendOp = AC_SRC_OVER;
	abrc.BF.BlendFlags = 0;
	abrc.BF.AlphaFormat = AC_SRC_ALPHA;// ignore source alpha channel 
	abrc.BF.SourceConstantAlpha = 0xFF;// use constant alpha, with 75% opaqueness  

	if (!AlphaBlend(abrc.HDCDST,
		abrc.RCDST.left, abrc.RCDST.top,
		abrc.RCDST.right - abrc.RCDST.left,
		abrc.RCDST.bottom - abrc.RCDST.top,
		abrc.HDCSRC,
		abrc.RCSRC.left, abrc.RCSRC.top,
		abrc.RCSRC.right - abrc.RCSRC.left,
		abrc.RCSRC.bottom - abrc.RCSRC.top,
		abrc.BF))
	{
		return NULL;// alpha blend failed 
	}

	//SaveBitmapToFile(abrc.HDCDST, hBitmap, _T("d:\\test.bmp"));
	
	// do cleanup 
	DeleteObject(hBitmap);
	DeleteDC(abrc.HDCSRC);
	abrc.HDCSRC = NULL;

	return hBitmap;
}

__inline static 
HFONT CreatePaintFont(const _TCHAR * ptszFaceName = _T("����"),
				LONG lfHeight = 12,
				LONG lfWidth = 0, 
				LONG lfEscapement = 0, 
				LONG lfOrientation = 0, 
				LONG lfWeight = FW_NORMAL, 
				BYTE lfItalic = FALSE,
				BYTE lfUnderline = FALSE, 
				BYTE lfStrikeOut = FALSE, 
				BYTE lfCharSet = ANSI_CHARSET, 
				BYTE lfOutPrecision = OUT_DEFAULT_PRECIS, 
				BYTE lfClipPrecision = CLIP_DEFAULT_PRECIS,
				BYTE lfQuality = DEFAULT_QUALITY, 
				BYTE lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS)
{
	return CreateFont(lfHeight, lfWidth,
			lfEscapement,
			lfOrientation,
			lfWeight,
			lfItalic,
			lfUnderline,
			lfStrikeOut,
			lfCharSet,
			lfOutPrecision,
			lfClipPrecision,
			lfQuality,
			lfPitchAndFamily,
			ptszFaceName);
}
__inline static
void RectDrawText(HDC hDC, RECT * pRect, const _TCHAR * ptszText, COLORREF clrTextColor = COLOR_WINDOWTEXT, HFONT hFont = NULL, UINT uFormat = DT_LEFT | DT_WORDBREAK)
{
	HFONT hFontOld = NULL;
	
	if (hFont)
	{
		hFontOld = (HFONT)SelectObject(hDC, hFont);
	}

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, clrTextColor);

	DrawText(hDC, ptszText, lstrlen(ptszText), pRect, uFormat);

	if (hFont)
	{
		(HFONT)SelectObject(hDC, hFontOld);
	}
}

__inline static BOOL DragMove(HWND hWnd, RECT * pRect)
{
	POINT pt = { 0 };
	BOOL bResult = FALSE;
	RECT rcWndOuter = { 0 };
	RECT rcWndInner = { 0 };
	::GetCursorPos(&pt);
	::ScreenToClient(hWnd, &pt);
	::GetClientRect(hWnd, &rcWndOuter);
	//::GetWindowRect(hWnd, &rcWndOuter);

	rcWndInner.left = rcWndOuter.left + pRect->left;
	rcWndInner.top = rcWndOuter.top + pRect->top;
	rcWndInner.right = rcWndOuter.right - pRect->right;
	rcWndInner.bottom = rcWndOuter.bottom - pRect->bottom;

	if (PtInRect(&rcWndOuter, pt) && !PtInRect(&rcWndInner, pt))
	{
#ifndef _SYSCOMMAND_SC_DRAGMOVE
#define _SYSCOMMAND_SC_DRAGMOVE  0xF012
#endif // !_SYSCOMMAND_SC_DRAGMOVE
		::SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, 0);
		::SendMessage(hWnd, WM_SYSCOMMAND, _SYSCOMMAND_SC_DRAGMOVE, 0);
		bResult = TRUE;
#ifdef _SYSCOMMAND_SC_DRAGMOVE
#undef _SYSCOMMAND_SC_DRAGMOVE  
#endif // !_SYSCOMMAND_SC_DRAGMOVE
	}

	return bResult;
}

__inline static BOOL FullDragMove(HWND hWnd, RECT * pRect)
{
	POINT pt = { 0 };
	BOOL bResult = FALSE;
	RECT rcWndOuter = { 0 };
	RECT rcWndInner = { 0 };
	::GetCursorPos(&pt);
	//::ScreenToClient(hWnd, &pt);
	//::GetClientRect(hWnd, &rcWndOuter);
	::GetWindowRect(hWnd, &rcWndOuter);

	rcWndInner.left = rcWndOuter.left + pRect->left;
	rcWndInner.top = rcWndOuter.top + pRect->top;
	rcWndInner.right = rcWndOuter.right - pRect->right;
	rcWndInner.bottom = rcWndOuter.bottom - pRect->bottom;

	//if (PtInRect(&rcWndOuter, pt) && !PtInRect(&rcWndInner, pt))
	{
#ifndef _SYSCOMMAND_SC_DRAGMOVE
#define _SYSCOMMAND_SC_DRAGMOVE  0xF012
#endif // !_SYSCOMMAND_SC_DRAGMOVE
		::SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, 0);
		::SendMessage(hWnd, WM_SYSCOMMAND, _SYSCOMMAND_SC_DRAGMOVE, 0);
		bResult = TRUE;
#ifdef _SYSCOMMAND_SC_DRAGMOVE
#undef _SYSCOMMAND_SC_DRAGMOVE  
#endif // !_SYSCOMMAND_SC_DRAGMOVE
	}

	return bResult;
}

__inline static
HGLOBAL OpenResource(LPVOID & lpData, DWORD & dwSize, HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType)
{ 
	BOOL bResult = FALSE;
	HRSRC hRsrcRes = NULL;// handle/ptr. to res. info. in hSource 
	HGLOBAL hGLOBAL = NULL;// handle to loaded resource
	
	// Locate the resource in the source image file. 
	hRsrcRes = ::FindResource(hModule, lpName, lpType);
	if (hRsrcRes == NULL)
	{
		goto __LEAVE_CLEAN__;
	}

	// Load the resource into global memory. 
	hGLOBAL = ::LoadResource(hModule, hRsrcRes);
	if (hGLOBAL == NULL)
	{
		goto __LEAVE_CLEAN__;
	}

	// Lock the resource into global memory. 
	lpData = ::LockResource(hGLOBAL);
	if (lpData == NULL)
	{
		FreeResource(hGLOBAL);
		hGLOBAL = NULL;
		goto __LEAVE_CLEAN__;
	}

	dwSize = ::SizeofResource(hModule, hRsrcRes);
	
__LEAVE_CLEAN__:

	return hGLOBAL;
}

__inline static 
void CloseResource(HGLOBAL & hGlobal)
{
	if (hGlobal)
	{
		FreeResource(hGlobal);
		hGlobal = NULL;
	}
}

__inline static 
BOOL ParseResrc(const _TCHAR * ptszFileName, UINT uResourceID, const _TCHAR * ptszTypeName)
{	
	DWORD dwSize = 0;
	HANDLE hFile = NULL;
	BOOL bResult = FALSE;
	LPVOID lpData = NULL;
	HGLOBAL hGlobal = NULL;
	DWORD dwByteWritten = 0;
		
	hGlobal = OpenResource(lpData, dwSize, GetModuleHandle(NULL), MAKEINTRESOURCE(uResourceID), ptszTypeName);

	//�����øղŵõ���pBuffer��dwSize����һЩ��Ҫ�����顣����ֱ�����ڴ���ʹ
	//�ã�Ҳ����д�뵽Ӳ���ļ����������Ǽ򵥵�д�뵽Ӳ���ļ���������ǵ��Զ�
	//����Դ����ΪǶ��DLL��Ӧ�ã��������Ҫ����һЩ��
	hFile = CreateFile(ptszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile && hGlobal && dwSize > 0)
	{
		WriteFile(hFile, hGlobal, dwSize, &dwByteWritten, NULL);
		CloseHandle(hFile);

		bResult = TRUE;
	}

	CloseResource(hGlobal);

	return bResult;
}

__inline static
BOOL IsFileExist(LPCTSTR fileName)
{
	WIN32_FIND_DATA	findData;

	HANDLE hFile = FindFirstFile(fileName, &findData);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
__inline static 
BOOL IsFileExistEx(LPCTSTR lpFileName)
{
	BOOL bResult = FALSE;
	GET_FILEEX_INFO_LEVELS gfil = GetFileExInfoStandard;
	WIN32_FILE_ATTRIBUTE_DATA wfad = { 0 };
	if (GetFileAttributes(lpFileName) != INVALID_FILE_ATTRIBUTES)
	{
		bResult = TRUE;
	}
	else
	{
		if (GetFileAttributesEx(lpFileName, gfil, &wfad) && 
			wfad.dwFileAttributes != INVALID_FILE_ATTRIBUTES)
		{
			bResult = TRUE;
		}
	}
	return bResult;
}

//����ʵ��ֻ����һ��
__inline static BOOL RunAppOnce(tstring tsName)
{
	HANDLE hMutexInstance = ::CreateMutex(NULL, FALSE, tsName.c_str());  //��������
	if (hMutexInstance)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			//OutputDebugString(_T("�����ⷵ�أ�"));
			CloseHandle(hMutexInstance);
			ExitProcess((0L));
			return FALSE;
		}
	}
	return TRUE;
}

__inline static //��ȡϵͳ·��
tstring GetSystemPath()
{
	tstring tsSystemPath = _T("");
	_TCHAR tSystemPath[MAX_PATH] = { 0 };
	GetSystemDirectory(tSystemPath, MAX_PATH);
	if (*tSystemPath)
	{
		tsSystemPath = tstring(tSystemPath) + _T("\\");
	}
	return tsSystemPath;
}

__inline static //��ȡϵͳ·��
tstring GetSystemPathX64()
{
	tstring tsSystemPath = _T("");
	_TCHAR tSystemPath[MAX_PATH] = { 0 };
	GetSystemWow64Directory(tSystemPath, MAX_PATH);
	if (*tSystemPath)
	{
		tsSystemPath = tstring(tSystemPath) + _T("\\");
	}
	return tsSystemPath;
}

//��ʼ�����Դ�����ʾ
__inline static void InitDebugConsole()
{
	FILE *pStdOut = stdout;
	FILE *pStdIn = stdin;
	FILE *pStdErr = stderr;

	if (!AllocConsole())
	{
		_TCHAR tErrorInfos[16384] = { 0 };
		_sntprintf(tErrorInfos, sizeof(tErrorInfos) / sizeof(_TCHAR), _T("����̨����ʧ��! �������:0x%X��"), GetLastError());
		MessageBox(NULL, tErrorInfos, _T("������ʾ"), 0);
		return;
	}
	SetConsoleTitle(_T("TraceDebugWindow"));

	pStdOut = _tfreopen(_T("CONOUT$"), _T("w"), stdout);
	pStdIn = _tfreopen(_T("CONIN$"), _T("r"), stdin);
	pStdErr = _tfreopen(_T("CONERR$"), _T("w"), stderr);
	_tsetlocale(LC_ALL, _T("chs"));
}

//�ͷŵ����Դ�����ʾ
__inline static void ExitDebugConsole()
{
	FreeConsole();
}

//	ANSI to Unicode
__inline static std::wstring ANSIToUnicode(const std::string str)
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t * pUnicode;
	pUnicode = new  wchar_t[(unicodeLen + 1)];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	std::wstring rt;
	rt = (wchar_t*)pUnicode;
	delete pUnicode;
	return rt;
}

//Unicode to ANSI
__inline static std::string UnicodeToANSI(const std::wstring str)
{
	char* pElementText;
	int iTextLen;
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}
//UTF - 8 to Unicode
__inline static std::wstring UTF8ToUnicode(const std::string str)
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t * pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	std::wstring rt;
	rt = (wchar_t*)pUnicode;
	delete pUnicode;
	return rt;
}
//Unicode to UTF - 8
__inline static std::string UnicodeToUTF8(const std::wstring str)
{
	char*   pElementText;
	int iTextLen;
	iTextLen = WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

__inline static std::string TToA(tstring tsT)
{
	std::string str = "";

#if !defined(UNICODE) && !defined(_UNICODE)
	str = tsT;
#else
	str = UnicodeToANSI(tsT);
#endif

	return str;
}

__inline static std::wstring TToW(tstring tsT)
{
	std::wstring wstr = L"";

#if !defined(UNICODE) && !defined(_UNICODE)
	wstr = ANSIToUnicode(tsT);
#else
	wstr = tsT;
#endif

	return wstr;
}

__inline static tstring AToT(std::string str)
{
	tstring ts = _T("");

#if !defined(UNICODE) && !defined(_UNICODE)
	ts = str;
#else
	ts = ANSIToUnicode(str);
#endif

	return ts;
}

__inline static tstring WToT(std::wstring wstr)
{
	tstring ts = _T("");

#if !defined(UNICODE) && !defined(_UNICODE)
	ts = UnicodeToANSI(wstr);
#else
	ts = wstr;
#endif

	return ts;
}

//utf8 ת Unicode
__inline static std::wstring Utf82Unicode(const std::string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION || widesize == 0)
	{
		return std::wstring(L"");
	}

	std::vector<wchar_t> resultstring(widesize);

	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		return std::wstring(L"");
	}

	return std::wstring(&resultstring[0]);
}

//unicode תΪ ascii
__inline static std::string WideByte2Acsi(std::wstring& wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION || asciisize == 0)
	{
		return std::string("");
	}
	std::vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);

	if (convresult != asciisize)
	{
		return std::string("");
	}

	return std::string(&resultstring[0]);
}

//utf-8 ת ascii
__inline static std::string UTF_82ASCII(std::string& strUtf8Code)
{
	std::string strRet("");
	//�Ȱ� utf8 תΪ unicode
	std::wstring wstr = Utf82Unicode(strUtf8Code);
	//���� unicode תΪ ascii
	strRet = WideByte2Acsi(wstr);
	return strRet;
}

///////////////////////////////////////////////////////////////////////


//ascii ת Unicode
__inline static std::wstring Acsi2WideByte(std::string& strascii)
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION || widesize == 0)
	{
		return std::wstring(L"");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);


	if (convresult != widesize)
	{
		return std::wstring(L"");
	}

	return std::wstring(&resultstring[0]);
}


//Unicode ת Utf8
__inline static std::string Unicode2Utf8(const std::wstring& widestring)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		return std::string("");
	}

	std::vector<char> resultstring(utf8size);

	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

	if (convresult != utf8size)
	{
		return std::string("");
	}

	return std::string(&resultstring[0]);
}

//ascii ת Utf8
__inline static std::string ASCII2UTF_8(std::string& strAsciiCode)
{
	std::string strRet("");
	//�Ȱ� ascii תΪ unicode
	std::wstring wstr = Acsi2WideByte(strAsciiCode);
	//���� unicode תΪ utf8
	strRet = Unicode2Utf8(wstr);
	return strRet;
}

#include <OlsDef.h>

#ifdef RUN_TIME_DYNAMIC_LINKING
#include <OlsApiInitExt.h>
#else // for Load-Time Dynamic Linking
#include <OlsApi.h>
#endif

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
