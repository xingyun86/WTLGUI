// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
	{
		SetBkMode(wndStatic.GetDC(), TRANSPARENT);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		RECT rc = { 6, 16, 6, 6 };
		memset(&m_rcOld, 0, sizeof(m_rcOld));
		memcpy(&m_rcBorder, &rc, sizeof(m_rcBorder));
		m_nFont = 14;
		m_strFont = _T("宋体");
		m_uARGB[0] = ARGB(0x7F, 0x00, 0xFF, 0x00);
		m_uARGB[1] = ARGB(0x7F, 0xFF, 0x00, 0x00);

		CenterWindow(GetParent());

		SetWindowLong(GWL_STYLE, GetWindowLong(GWL_STYLE) &(~WS_CAPTION) &(~WS_THICKFRAME));

		SetDlgItemText(IDC_STATIC, _T("系统温度监控工具 v1.0\nxingyun86(523381005)\n(c) Copyright 2017"));
		::ShowWindow(GetDlgItem(IDC_STATIC), SW_HIDE);
		SetDlgItemText(IDOK, _T("关闭"));

		return TRUE;
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
			//SetBkMode(hDC, TRANSPARENT);
			DrawMemoryBitmap(hDC, this->m_hWnd,
				rc.right - rc.left,
				rc.bottom - rc.top,
				DrawAlphaBlendRect(this->m_hWnd, m_uARGB, hDC, &m_rcBorder));

			SetRect(&rc, 20, 100, 180, 200);
			hFontNew = CreatePaintFont(m_strFont, m_nFont);
			if (hFontNew)
			{
				RectDrawText(hDC, &rc, _T("系统温度监控工具 v1.0\nxingyun86(523381005)\n(c) Copyright 2017"), RGB(255, 255, 255), hFontNew, DT_CENTER | DT_WORDBREAK);
			}

			EndPaint(&ps);
		}

		return 0;
	}
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		DragMove(this->m_hWnd, &m_rcBorder);

		return 0;
	}
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

private:
	INT m_nFont;
	RECT m_rcOld;
	RECT m_rcBorder;
	ULONG m_uARGB[2];
	CString m_strFont;
};
