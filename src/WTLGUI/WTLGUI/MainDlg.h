// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TrayIconImpl.h"
#include "ProcessManager.h"

class CMainDlg : 
	public CTrayIconImpl<CMainDlg>,
	public CDialogImpl<CMainDlg>, 
	public CUpdateUI<CMainDlg>,
	public CMessageFilter, 
	public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateChildWindows();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MSG_WM_TIMER(OnTimer)
		MESSAGE_HANDLER(WM_NCMOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		//MSG_WM_SYSCOMMAND(OnSysCommand)
		COMMAND_ID_HANDLER(IDM_MAINTRAY_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(IDM_MAINTRAY_SHOW, OnTrayShow)
		COMMAND_ID_HANDLER_EX(IDM_MAINTRAY_HIDE, OnTrayHide)
		CHAIN_MSG_MAP(CTrayIconImpl<CMainDlg>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	void OnSysCommand(UINT nID, CPoint point)
	{
		switch (nID)
		{
		case IDM_MAINTRAY_ABOUT:
		{
			CAboutDlg dlg;
			dlg.DoModal();
		}
		break;
		default:
		{
			//no-op
			SetMsgHandled(FALSE);
		}
		break;
		}
	}
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		RECT rc = { 16, 16, 16, 16 };
		memset(&m_rcOld, 0, sizeof(m_rcOld));
		memcpy(&m_rcBorder, &rc, sizeof(m_rcBorder));

		m_nFont = 20;
		m_strFont = _T("宋体");
		m_uARGB[0] = ARGB(0x7F, 0x00, 0x00, 0xFF);
		m_uARGB[1] = ARGB(0x7F, 0x00, 0xFF, 0x00);

		NearSideAutoHide::InitScreenSize();

		// center the dialog on the screen
		CenterWindow();
				
		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);
		
		INT nLineWidth = 0;
		INT nFit = 0;
		INT nDx = 0;
		SIZE cSize = { 0 };
		int nLines = 0;
		int nStart = 0;
		int nFinal = 0;
		CString strMax(_T(""));
		CString strFind(_T("\r\n"));
		CString strInfo = GetSystemTemperatureInfo();
		if (strInfo.GetLength())
		{
			while ((nFinal = strInfo.Find(strFind, nStart)) >= 0)
			{
				CString strTmp = strInfo.Mid(nStart, nFinal - nStart);
				if (strTmp.GetLength() > strMax.GetLength())
				{
					strMax = strTmp;
				}
				nStart = nFinal + strFind.GetLength();
				nLines++;
			}
			
			GetTextExtentExPoint(GetDC(), strMax, strMax.GetLength(), nLineWidth, &nFit, &nDx, &cSize);
			SetWindowPos(HWND_TOPMOST, 0, 0, cSize.cx * 2, m_nFont * (nLines + 2), SWP_NOMOVE);
		}
		else
		{
			SetWindowPos(HWND_TOPMOST, 0, 0, 300, 200, SWP_NOMOVE);
		}
		SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) &(~WS_CAPTION) &(~WS_THICKFRAME));
		
		::ShowWindow(GetDlgItem(IDOK), SW_HIDE);
		::ShowWindow(GetDlgItem(IDCANCEL), SW_HIDE); 
		::ShowWindow(GetDlgItem(ID_APP_ABOUT), SW_HIDE);

		m_menuTrayIcon.LoadMenu(IDR_MENU);

		// Install the tray icon
		InstallIcon(_T("TRAYICON"), hIconSmall, IDR_MENU);

		SetTimer(1, 750);
		
		return TRUE;
	}
	
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		NearSideAutoHide::OnMouseMove(this->m_hWnd);
		return 0;
	}
	LRESULT OnEnterSizeMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		NearSideAutoHide::OnEnterSizeMove(this->m_hWnd);
		return 0;
	}
	LRESULT OnExitSizeMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		NearSideAutoHide::OnExitSizeMove(this->m_hWnd);
		return 0;
	}
	LRESULT OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		NearSideAutoHide::OnMoving(this->m_hWnd, lParam);
		return 0;
	}
	CString GetSystemTemperatureInfo()
	{
		CString strInfo(_T(""));
		WSTRINGVECTORMAP wsvmap;

		CProcessManager::GetSystemHardwareInfoSync(wsvmap, _T("SELECT * FROM Win32_TemperatureProbe"), _T("root\\CIMV2"));
		if (!wsvmap.size())
		{
			CProcessManager::GetSystemHardwareInfoSync(wsvmap, _T("SELECT * FROM MSAcpi_ThermalZoneTemperature"), _T("root\\WMI"));
		}
		if (wsvmap.size())
		{
			double dValue = (double)(_tcstoul(wsvmap[_T("CurrentTemperature")].at(0).c_str(), 0, 10) * 10 - 27315) / 100;
			strInfo.AppendFormat(_T("主板-温度: %.2f℃\r\n"), dValue, wsvmap[_T("CurrentTemperature")].at(0).c_str());
		}
		wsvmap.clear();
		CProcessManager::GetSystemHardwareInfoSync(wsvmap, _T("SELECT * FROM MSStorageDriver_ATAPISmartData"), _T("root\\WMI"));
		if (wsvmap.size())
		{
			size_t stIdx = 0;
			for (stIdx = 0; stIdx < wsvmap.begin()->second.size(); stIdx++)
			{
				ULONG nValue = 0;
				std::wstring wstr = wsvmap[_T("VendorSpecific")].at(stIdx).c_str();
				std::wstring wstrValue = wstr.substr(1 + 151 * 5, 4);
				std::wstring::size_type stPos = wstr.find(_T("0xC2,0x22,"));
				if (stPos != std::wstring::npos)
				{
					wstrValue = wstr.substr(stPos + 5 * 5, 4);
					nValue = _tcstoul(wstrValue.c_str(), NULL, 16);
				}
				strInfo.AppendFormat(_T("硬盘%d温度: %.2f℃\r\n"), stIdx + 1, (double)nValue);
			}
		}

		DWORD dwIdx = 0;
		DWORD dwResult = 0;
		DWORD dwCpuIdx = 0;
		DWORD dwEAX, dwEBX, dwECX, dwEDX;
		int nDTS = 0;
		int nTjunction = 0;
		float m_fCPUTemp = 0.0f;
		BOOL bResult = FALSE;

		SYSTEM_INFO si = { 0 };
		GetNativeSystemInformation(si);
		for (dwIdx = 0; dwIdx < si.dwNumberOfProcessors; dwIdx++)
		{
			dwCpuIdx = dwIdx + 1;
			bResult = Rdmsr(0x1a2, &dwEAX, &dwEDX);
			nTjunction = (dwEAX >> 16 & 255);
			dwResult = SetThreadAffinityMask(GetCurrentThread(), dwCpuIdx);
			bResult = Rdmsr(0x19c, &dwEAX, &dwEDX);
			dwResult = SetThreadAffinityMask(GetCurrentThread(), dwResult);
			nDTS = (dwEAX >> 16 & 255);
			m_fCPUTemp = (float)(nTjunction - nDTS + 100);
			strInfo.AppendFormat(_T("CPU%d-温度: %.2f℃\r\n"), dwCpuIdx, m_fCPUTemp);
		}

		return strInfo;
	}
	void OnTimer(UINT uTimerID)
	{
		switch (uTimerID)
		{
		case 1:
		{
			CString strInfo = GetSystemTemperatureInfo();
			
			SetWindowText(strInfo);
			m_strInfo = strInfo;
			NotifyUpdate(this->m_hWnd, m_rcOld, TRUE);
			RedrawWindow();
		}
		break;
		default:
		{
			if (!NearSideAutoHide::OnTimer(this->m_hWnd, uTimerID))
			{
				SetMsgHandled(FALSE);
			}
		}			
		break;
		}
	}
	void OnTrayHide(UINT, int, HWND)
	{
		// Hide the dialog
		ShowWindow(SW_HIDE);
	}
	void OnTrayShow(UINT, int, HWND)
	{
		BOOL bRet = FALSE;
		CString strInfo(_T(""));
		m_menuTrayIcon.GetMenuString(IDM_MAINTRAY_SHOW, strInfo, MF_BYCOMMAND);

		MENUITEMINFO mii;
		TCHAR szItemData[256];
		memset(&mii, 0, sizeof(MENUITEMINFO));
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_TYPE;
		mii.fType = MFT_STRING;
		mii.cch = sizeof(szItemData) / sizeof(_TCHAR);
		mii.dwTypeData = szItemData;

		bRet = m_menuTrayIcon.GetMenuItemInfo(IDM_MAINTRAY_SHOW, FALSE, &mii);

		_tcscpy(szItemData, _T("Show"));
		memset(&mii, 0, sizeof(MENUITEMINFO));
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_TYPE;
		mii.fType = MFT_STRING;
		mii.cch = sizeof(szItemData) / sizeof(_TCHAR);
		mii.dwTypeData = szItemData;
		m_menuTrayIcon.SetMenuItemInfo(IDM_MAINTRAY_SHOW, FALSE, &mii);

		// Show dialog
		if (!IsWindowVisible())
		{
			ShowWindow(SW_SHOW);
		}
		// Restore if window if minimized
		if (IsIconic())
		{
			ShowWindow(SW_RESTORE);
		}
		else
		{
			BringWindowToTop();
		}
		// Make this the active window
		::SetForegroundWindow(m_hWnd);
	}
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = 0;
		return 0;
	}
	
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		DragMove(this->m_hWnd, &m_rcBorder);

		return 0;
	}
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		RECT rc = { 0 };
		LOGFONT lf = { 0 };
		HFONT hFontNew = NULL;
		PAINTSTRUCT ps = { 0 };
		HDC hDC = BeginPaint(&ps);
		if (hDC)
		{
			//NotifyUpdate(this->m_hWnd, m_rcOld, TRUE);

			GetClientRect(&rc);
				
			DrawMemoryBitmap(hDC, this->m_hWnd, 
				rc.right - rc.left, 
				rc.bottom - rc.top,
				DrawAlphaBlendRect(this->m_hWnd, m_uARGB, hDC, &m_rcBorder));
			
			hFontNew = CreatePaintFont(m_strFont, m_nFont);
			if (hFontNew)
			{
				rc.left = rc.left + m_rcBorder.left;
				rc.top = rc.top + m_rcBorder.top;
				rc.right = rc.right - m_rcBorder.right;
				rc.bottom = rc.bottom - m_rcBorder.bottom;

				RectDrawText(hDC, &rc, m_strInfo, RGB(255, 255, 0), hFontNew, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
				SetTooltipText(m_strInfo);

				DeleteObject(hFontNew);
				hFontNew = NULL;
			}			

			EndPaint(&ps);
		}		
		
		return 0;
	}

	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		NotifyUpdate(this->m_hWnd, m_rcOld, TRUE);
		return 0;
	}

	LRESULT OnSizing(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		NotifyUpdate(this->m_hWnd, m_rcOld, TRUE);
		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

private:
	INT m_nFont;
	RECT m_rcOld;
	RECT m_rcBorder;

	ULONG m_uARGB[2];
	CString m_strFont;
	CString m_strInfo;

	CMenu m_menuTrayIcon;
};
