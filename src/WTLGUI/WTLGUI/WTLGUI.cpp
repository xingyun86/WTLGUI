// WTLGUI.cpp : main source file for WTLGUI.exe
//

#include "stdafx.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

CAppModule _Module;

#ifdef RUN_TIME_DYNAMIC_LINKING
HMODULE m_hOpenLibSys;
#endif

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{	
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	RunAppOnce(_T("__WTLGUI__MODULE__ONCE__"));

	//HRESULT hResult = ::CoInitialize(NULL);
	HRESULT hResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------

	hResult = CoInitializeSecurity(NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL);                       // Reserved

	// If you are running on NT 4.0 or higher you can use the following call instead to 
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hResult));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hResult = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hResult));
		
#ifdef RUN_TIME_DYNAMIC_LINKING
	m_hOpenLibSys = NULL;
	if (!InitOpenLibSys(&m_hOpenLibSys))
	{
		MessageBox(NULL, _T("DLL Load Error!!"), _T("Message"), MB_ICONINFORMATION);
		return FALSE;
}
#else
	if (!InitializeOls())
	{
		MessageBox(NULL, _T("Error InitializeOls()!"), _T("Message"), MB_ICONINFORMATION);
		return FALSE;
	}
#endif

	int nRet = Run(lpstrCmdLine, nCmdShow);

#ifdef RUN_TIME_DYNAMIC_LINKING
	DeinitOpenLibSys(&m_hOpenLibSys);
#else
	DeinitializeOls();
#endif

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
