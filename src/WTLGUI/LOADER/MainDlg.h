// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define XX64_NAME	"WINRING0X64.EXE"
#define XX32_NAME	"WINRING0.EXE"
#define X64X_NAME	"WINRING0X64.DLL"
#define X32X_NAME	"WINRING0.DLL"
#define DX64_NAME	"WINRING0X64.SYS"
#define DX32_NAME	"WINRING0.SYS"
#define DVXD_NAME	"WINRING0.VXD"

class CMainDlg : 
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
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		RECT rc = { 16, 16, 16, 16 };
		memset(&m_rcOld, 0, sizeof(m_rcOld));
		memcpy(&m_rcBorder, &rc, sizeof(m_rcBorder));

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
		
		SetWindowPos(HWND_TOPMOST, 0, 0, 228, 132, SWP_NOMOVE);
		SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) &(~WS_CAPTION) &(~WS_THICKFRAME));
		
		//::ShowWindow(GetDlgItem(IDOK), SW_HIDE);
		::ShowWindow(GetDlgItem(IDCANCEL), SW_HIDE); 
		::ShowWindow(GetDlgItem(ID_APP_ABOUT), SW_HIDE);

		{
			if (!IsFileExistEx(tstring(GetSystemPath() + _T(XX64_NAME)).c_str()))
			{
				if (!ParseResrc(tstring(GetSystemPath() + _T(XX64_NAME)).c_str(), IDR_IDC_XX641, _T("IDC_XX64")))
				{
					//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
					return 0;
				}
				//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
			}
			if (!IsFileExistEx(tstring(GetSystemPath() + _T(XX32_NAME)).c_str()))
			{
				if (!ParseResrc(tstring(GetSystemPath() + _T(XX32_NAME)).c_str(), IDR_IDC_XX321, _T("IDC_XX32")))
				{
					//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
					return 0;
				}
				//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
			}

			if (!IsFileExistEx(tstring(GetSystemPath() + _T(X64X_NAME)).c_str()))
			{
				if (!ParseResrc(tstring(GetSystemPath() + _T(X64X_NAME)).c_str(), IDR_IDC_X64X1, _T("IDC_X64X")))
				{
					//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
					return 0;
				}
				//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
			}
			if (!IsFileExistEx(tstring(GetSystemPath() + _T(X32X_NAME)).c_str()))
			{
				if (!ParseResrc(tstring(GetSystemPath() + _T(X32X_NAME)).c_str(), IDR_IDC_X32X1, _T("IDC_X32X")))
				{
					//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
					return 0;
				}
				//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
			}

			if (!IsFileExistEx(tstring(GetSystemPath() + _T(DX64_NAME)).c_str()))
			{
				if (!ParseResrc(tstring(GetSystemPath() + _T(DX64_NAME)).c_str(), IDR_IDC_DX641, _T("IDC_DX64")))
				{
					//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
					return 0;
				}
				//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
			}

			if (!IsFileExistEx(tstring(GetSystemPath() + _T(DX32_NAME)).c_str()))
			{
				if (!ParseResrc(tstring(GetSystemPath() + _T(DX32_NAME)).c_str(), IDR_IDC_DX321, _T("IDC_DX32")))
				{
					//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
					return 0;
				}
				//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
			}
			if (!IsFileExistEx(tstring(GetSystemPath() + _T(DVXD_NAME)).c_str()))
			{
				if (!ParseResrc(tstring(GetSystemPath() + _T(DVXD_NAME)).c_str(), IDR_IDC_DVXD1, _T("IDC_DVXD")))
				{
					//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
					return 0;
				}
				//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
			}
		}
		
		SYSTEM_INFO si = { 0 };
		GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
			si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		{
			{
				if (!IsFileExistEx(tstring(GetSystemPathX64() + _T(XX64_NAME)).c_str()))
				{
					if (!ParseResrc(tstring(GetSystemPathX64() + _T(XX64_NAME)).c_str(), IDR_IDC_XX641, _T("IDC_XX64")))
					{
						//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
						return 0;
					}
					//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
				}
				if (!IsFileExistEx(tstring(GetSystemPathX64() + _T(XX32_NAME)).c_str()))
				{
					if (!ParseResrc(tstring(GetSystemPathX64() + _T(XX32_NAME)).c_str(), IDR_IDC_XX321, _T("IDC_XX32")))
					{
						//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
						return 0;
					}
					//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
				}

				if (!IsFileExistEx(tstring(GetSystemPathX64() + _T(X64X_NAME)).c_str()))
				{
					if (!ParseResrc(tstring(GetSystemPathX64() + _T(X64X_NAME)).c_str(), IDR_IDC_X64X1, _T("IDC_X64X")))
					{
						//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
						return 0;
					}
					//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
				}
				if (!IsFileExistEx(tstring(GetSystemPathX64() + _T(X32X_NAME)).c_str()))
				{
					if (!ParseResrc(tstring(GetSystemPathX64() + _T(X32X_NAME)).c_str(), IDR_IDC_X32X1, _T("IDC_X32X")))
					{
						//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
						return 0;
					}
					//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
				}

				if (!IsFileExistEx(tstring(GetSystemPathX64() + _T(DX64_NAME)).c_str()))
				{
					if (!ParseResrc(tstring(GetSystemPathX64() + _T(DX64_NAME)).c_str(), IDR_IDC_DX641, _T("IDC_DX64")))
					{
						//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
						return 0;
					}
					//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
				}

				if (!IsFileExistEx(tstring(GetSystemPathX64() + _T(DX32_NAME)).c_str()))
				{
					if (!ParseResrc(tstring(GetSystemPathX64() + _T(DX32_NAME)).c_str(), IDR_IDC_DX321, _T("IDC_DX32")))
					{
						//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
						return 0;
					}
					//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
				}
				if (!IsFileExistEx(tstring(GetSystemPathX64() + _T(DVXD_NAME)).c_str()))
				{
					if (!ParseResrc(tstring(GetSystemPathX64() + _T(DVXD_NAME)).c_str(), IDR_IDC_DVXD1, _T("IDC_DVXD")))
					{
						//MessageBox(NULL, _T("解压文件失败!"), _T("提示"), MB_ICONINFORMATION);
						return 0;
					}
					//MessageBox(NULL, _T("解压文件成功!"), _T("提示"), MB_ICONINFORMATION);
				}
			}
			WinExec(CStringA(tstring(GetSystemPathX64() + _T(XX64_NAME)).c_str()), SW_SHOW);
		}
		else
		{
			WinExec(CStringA(tstring(GetSystemPath() + _T(XX32_NAME)).c_str()), SW_SHOW);
		}
		ExitProcess(IDCLOSE);

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
	void OnTimer(UINT uTimerID)
	{
		switch (uTimerID)
		{
		case 1:
		{
			
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
			NotifyUpdate(this->m_hWnd, m_rcOld, TRUE);

			GetClientRect(&rc);
				
			DrawMemoryBitmap(hDC, this->m_hWnd, 
				rc.right - rc.left, 
				rc.bottom - rc.top,
				DrawAlphaBlendRect(this->m_hWnd, hDC, &m_rcBorder));
			
			hFontNew = CreatePaintFont(_T("宋体"), 20);
			if (hFontNew)
			{
				rc.left = rc.left + m_rcBorder.left;
				rc.top = rc.top + m_rcBorder.top;
				rc.right = rc.right - m_rcBorder.right;
				rc.bottom = rc.bottom - m_rcBorder.bottom;

				RectDrawText(hDC, &rc, m_strInfo, RGB(255, 255, 0), hFontNew);
				
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
	RECT m_rcOld;
	RECT m_rcBorder;

	CString m_strInfo;

	CMenu m_menuTrayIcon;
};
