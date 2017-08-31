#pragma once

#include "stdafx.h"

typedef std::vector<std::wstring> WSTRINGVECTOR;
typedef WSTRINGVECTOR::iterator WSTRINGVECTORIT;

typedef std::map<std::wstring, WSTRINGVECTOR> WSTRINGVECTORMAP;
typedef WSTRINGVECTORMAP::iterator WSTRINGVECTORMAPIT;
typedef WSTRINGVECTORMAP::value_type WSTRINGVECTORMAPPAIR;
static void VariantToCString(CString & str, VARIANT var)
{
	VARIANT varBSTR = { 0 };
	double dDecVal = 0.0f;
	SYSTEMTIME st = { 0 };
	CString strFormat(_T(""));
	switch (var.vt)
	{
	case VT_EMPTY: // 0
		str.Format(_T(""));
		break;

	case VT_NULL: // 1
		str.Format(_T(""));
		break;

	case VT_I2: // 2
		str.Format(_T("%d"), var.iVal);
		break;

	case VT_I4: // 3
		str.Format(_T("%ld"), var.intVal);
		break;

	case VT_R4: // 4
		str.Format(_T("%10.6f"), (double)var.fltVal);
		break;

	case VT_R8: // 5
		str.Format(_T("%10.6f"), var.dblVal);
		break;

	case VT_CY: // 6
		varBSTR.vt = VT_BSTR;
		varBSTR.bstrVal = L"";
		VariantChangeType(&varBSTR, &var, VARIANT_NOVALUEPROP, varBSTR.vt);
		str.Format(_T("%10.6f"), varBSTR.dblVal);
		break;

	case VT_DATE: // 7
		VariantTimeToSystemTime(var.date, &st);
		str.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%d"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		break;

	case VT_BSTR: // 8
		str = var.bstrVal;
		break;

	case VT_DISPATCH: // 9
		str.Format(_T("0x%lX"), var.pdispVal);
		break;

	case VT_ERROR: // 10
		str.Format(_T("%ld"), var.scode);
		break;

	case VT_BOOL: // 11
		str.Format(_T("%d"), varBSTR.boolVal);
		break;

	case VT_VARIANT: // 12
		str.Format(_T("0x%lX"), var.pvarVal);
		break;

	case VT_UNKNOWN: // 13
		str.Format(_T("Unknow Ptr:0x%lX"), var.pvarVal);
		break;

	case VT_DECIMAL: // 14
		dDecVal = var.decVal.Lo64;
		dDecVal *= (var.decVal.sign == 128) ? -1 : 1;
		dDecVal /= pow(10, var.decVal.scale);

		strFormat.Format(_T("%d"), var.decVal.scale);
		strFormat = _T("%.") + strFormat + _T("f");
		str.Format(strFormat, dDecVal);
		break;

	case VT_I1: // 16
		str.Format(_T("%C(0x%02X)"), var.cVal, var.cVal);
		break;

	case VT_UI1: // 17
		str.Format(_T("%C(0x%02X)"), var.cVal, var.cVal);
		break;

	case VT_UI2: // 18
		str.Format(_T("%u"), var.uiVal);
		break;

	case VT_UI4: // 19
		str.Format(_T("%u"), var.uintVal);
		break;

	case VT_I8: // 20
		str.Format(_T("%lld"), var.llVal);
		break;

	case VT_UI8: // 21
		str.Format(_T("%llu"), var.ullVal);
		break;

	case VT_INT: // 22
		str.Format(_T("%ld"), var.intVal);
		break;

	case VT_UINT: // 23
		str.Format(_T("%lu"), var.uintVal);
		break;

	case VT_VOID: // 24
		str.Format(_T(""));
		break;

	case VT_HRESULT: // 25
		str.Format(_T(""));
		break;

	case VT_PTR: // 26
		str.Format(_T(""));
		break;

	case VT_SAFEARRAY: // 27
		//str.Format(_T(""));
		{
			HRESULT hResult = S_FALSE;
			LONG lIdx = 0;
			LONG lLbound = 0;
			LONG lUbound = 0;
			BSTR bsPropName = NULL;
			hResult = SafeArrayGetLBound(var.parray, 1, &lLbound);
			hResult = SafeArrayGetUBound(var.parray, 1, &lUbound);

			str.AppendFormat(_T("["));
			for (lIdx = lLbound; lIdx <= lUbound; lIdx++) {
				// Get this property name.
				hResult = SafeArrayGetElement(var.parray, &lIdx, &bsPropName);
				if (lIdx != lLbound)
				{
					str.AppendFormat(_T(","));
				}

				if (FAILED(hResult))
				{
					str.AppendFormat(_T(""));
				}
				else
				{
					str.AppendFormat(_T("%s"), bsPropName);
				}
			}
			str.AppendFormat(_T("]"));
			//hResult = SafeArrayDestroy(var.parray);
			//var.parray = NULL;
		}
		break;

	case VT_CARRAY: // 28
		str.Format(_T(""));
		break;

	case VT_USERDEFINED: // 29
		str.Format(_T(""));
		break;

	case VT_LPSTR: // 30
		str = var.pcVal;
		break;

	case VT_LPWSTR: // 31
		str = var.bstrVal;
		break;

	case VT_RECORD: // 36
		str.Format(_T(""));
		break;

	case VT_INT_PTR: // 37
		str.Format(_T(""));
		break;

	case VT_UINT_PTR: // 38
		str.Format(_T(""));
		break;

	case VT_FILETIME: // 64
		str.Format(_T(""));
		break;

	case VT_BLOB: // 65
		str.Format(_T(""));
		break;

	case VT_STREAM: // 66
		str.Format(_T(""));
		break;

	case VT_STORAGE: // 67
		str.Format(_T(""));
		break;

	case VT_STREAMED_OBJECT: // 68
		str.Format(_T(""));
		break;

	case VT_STORED_OBJECT: // 69
		str.Format(_T(""));
		break;

	case VT_BLOB_OBJECT: // 70
		str.Format(_T(""));
		break;

	case VT_CF: // 71
		str.Format(_T(""));
		break;

	case VT_CLSID: // 72
		str.Format(_T(""));
		break;

	case VT_VERSIONED_STREAM: // 73
		str.Format(_T(""));
		break;

	case VT_BSTR_BLOB: // 0xfff
		str.Format(_T(""));
		break;

	case VT_VECTOR: // 0x1000
		str.Format(_T(""));
		break;
	case VT_ARRAY: // 0x2000
		str.Format(_T(""));
		break;
	case VT_ARRAY | VT_UI1: // 0x2011
		{
			HRESULT hResult = S_FALSE;
			LONG lIdx = 0;
			LONG lLbound = 0;
			LONG lUbound = 0;
			BYTE bValue = NULL;
			hResult = SafeArrayGetLBound(var.parray, 1, &lLbound);
			hResult = SafeArrayGetUBound(var.parray, 1, &lUbound);

			str.AppendFormat(_T("["));
			for (lIdx = lLbound; lIdx <= lUbound; lIdx++) {
				// Get this property name.
				hResult = SafeArrayGetElement(var.parray, &lIdx, &bValue);
				if (lIdx != lLbound)
				{
					str.AppendFormat(_T(","));
				}

				if (FAILED(hResult))
				{
					str.AppendFormat(_T(""));
				}
				else
				{
					str.AppendFormat(_T("0x%02X"), bValue);
				}
			}
			str.AppendFormat(_T("]"));
			//hResult = SafeArrayDestroy(var.parray);
			//var.parray = NULL;
		}
		break;

	case VT_BYREF: // 0x4000
		str.Format(_T(""));
		break;

	case VT_RESERVED: // 0x8000
		str.Format(_T(""));
		break;

	case VT_ILLEGAL: // 0xffff
		str.Format(_T(""));
		break;

		//case VT_ILLEGALMASKED: // 0xfff
		//	break;
		//case VT_TYPEMASK: // 0xfff
		//	break;
	default:
		str.Format(_T(""));
		break;
	}
}
class CWbemObjectQuerySink : public IWbemObjectSink
{
	LONG m_lRef;
	bool bDone;
	CRITICAL_SECTION threadLock; // for thread safety

public:
	CWbemObjectQuerySink() {
		m_lRef = 0; bDone = false;
		InitializeCriticalSection(&threadLock);
	}
	~CWbemObjectQuerySink() {
		bDone = true;
		DeleteCriticalSection(&threadLock);
	}
	WSTRINGVECTORMAP m_wvmap;
	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return InterlockedIncrement(&m_lRef);
	}
	virtual ULONG STDMETHODCALLTYPE Release()
	{
		LONG lRef = InterlockedDecrement(&m_lRef);
		if (lRef == 0)
			delete this;
		return lRef;
	}
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
		void** ppv)
	{
		if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
		{
			*ppv = (IWbemObjectSink *) this;
			AddRef();
			return WBEM_S_NO_ERROR;
		}
		else return E_NOINTERFACE;
	}
	
	virtual HRESULT STDMETHODCALLTYPE Indicate(
		LONG lObjectCount,
		IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
	)
	{
		HRESULT hResult = S_OK;
		WSTRINGVECTOR wvector;
		BSTR bsPropName;
		SAFEARRAY * psa = NULL;
		hResult = (*apObjArray)->GetNames(NULL,	WBEM_FLAG_ALWAYS | WBEM_FLAG_NONSYSTEM_ONLY, NULL, &psa);
		if (SUCCEEDED(hResult))
		{
			LONG lLbound = 0;
			LONG lUbound = 0;
			hResult = SafeArrayGetLBound(psa, 1, &lLbound);
			hResult = SafeArrayGetUBound(psa, 1, &lUbound);

			for (LONG i = lLbound; i <= lUbound; i++) {
				// Get this property name.
				hResult = SafeArrayGetElement(psa, &i, &bsPropName);
				m_wvmap.insert(WSTRINGVECTORMAPPAIR(bsPropName, wvector));
				//len = (unsigned)SysStringLen(bsPropName);
				//length[0] = MY_MAX(length[0], len);
			} // enfor i

			hResult = SafeArrayDestroy(psa);
			psa = NULL;
		}
		for (int i = 0; i < lObjectCount; i++)
		{
			CString str(_T(""));
			VARIANT varName;
			WSTRINGVECTORMAPIT itEnd = m_wvmap.end();
			WSTRINGVECTORMAPIT itIdx = m_wvmap.begin();
			for (; itIdx != itEnd; itIdx++)
			{
				hResult = apObjArray[i]->Get((LPCWSTR)(itIdx->first.c_str()), 0, &varName, 0, 0);
				switch (varName.vt)
				{
				case VT_NULL:
					itIdx->second.push_back((L""));
					break;
				case VT_BSTR:
					itIdx->second.push_back(V_BSTR(&varName));
					break;
				default:
					VariantToCString(str, varName);
					itIdx->second.push_back((LPCWSTR)str);
					break;
				}
				if (FAILED(hResult))
				{
					printf("Failed to get the data from the query  Error code = 0x%X\n", hResult);
					//return WBEM_E_FAILED;       // Program has failed.
				}
			}
		}

		return WBEM_S_NO_ERROR;
	}

	virtual HRESULT STDMETHODCALLTYPE SetStatus(
		/* [in] */ LONG lFlags,
		/* [in] */ HRESULT hResult,
		/* [in] */ BSTR strParam,
		/* [in] */ IWbemClassObject __RPC_FAR *pObjParam
	)
	{
		if (lFlags == WBEM_STATUS_COMPLETE)
		{
			printf("Call complete.\n");

			EnterCriticalSection(&threadLock);
			bDone = true;
			LeaveCriticalSection(&threadLock);
		}
		else if (lFlags == WBEM_STATUS_PROGRESS)
		{
			printf("Call in progress.\n");
		}

		return WBEM_S_NO_ERROR;
	}

	bool IsDone()
	{
		bool done = true;

		EnterCriticalSection(&threadLock);
		done = bDone;
		LeaveCriticalSection(&threadLock);

		return done;
	}
};
class CProcessManager
{
public:

	CProcessManager()
	{
	}

	~CProcessManager()
	{
	}

	void VarToStr(CString & str, VARIANT var)
	{
		VARIANT varDest = { 0 };
		varDest.vt = VT_BSTR;
		varDest.bstrVal = BSTR(L"");
		HRESULT hr = ::VariantChangeType(&varDest, &var, VARIANT_NOUSEROVERRIDE | VARIANT_LOCALBOOL, VT_BSTR);
		str = varDest.bstrVal;
	}
	static CString ParseError(DWORD dwErrorCodes, HINSTANCE hInstance = NULL)
	{
		BOOL bResult = FALSE;
		CString strErrorText(_T(""));
		HLOCAL hLocal = NULL;

		bResult = ::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			hInstance,
			dwErrorCodes,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
			(LPTSTR)&hLocal,
			0,
			NULL);
		if (!bResult)
		{ 
			if (hInstance)
			{
				bResult = ::FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_HMODULE |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					hInstance,
					dwErrorCodes,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					(LPTSTR)&hLocal,
					0,
					NULL);
				if (!bResult)
				{
					// failed
					// Unknown error code %08x (%d)
					strErrorText.Format(_T("Unknown error code 0x%08X"), dwErrorCodes);
				}
			}
		}

		if(bResult && hLocal)
		{ 
			// success
			LPTSTR pT = StrStrI((LPCTSTR)hLocal, _T("\r"));
			if (pT != NULL)
			{ 
				//lose CRLF
				*pT = _T('\0');
			}
			strErrorText = (LPCTSTR)hLocal;
		}

		if (hLocal)
		{
			::LocalFree(hLocal);
			hLocal = NULL;
		}

		return strErrorText;
	}

	static void ShowMsgs(const _TCHAR * ptTitle, const _TCHAR * ptFormat, ...)
	{
		va_list arglist = 0;
		CString strText(_T(""));

		va_start(arglist, ptFormat);
		strText.FormatV(ptFormat, arglist);
		va_end(arglist);

		::MessageBox(NULL, strText, ptTitle, MB_ICONINFORMATION);
	}

	static BOOL PromotePrivilege(const _TCHAR * ptName = SE_DEBUG_NAME, BOOL bEnable = TRUE)
	{
		// 附给本进程特权,以便访问系统进程
		LUID uID = { 0 };
		HANDLE hToken = NULL;
		TOKEN_PRIVILEGES tp = { 0 };

		// 打开一个进程的访问令牌
		if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			return FALSE;
		}

		// 取得特权名称为"SetDebugPrivilege"的LUID
		if (!::LookupPrivilegeValue(NULL, ptName, &uID))
		{
			return FALSE;
		}
			
		// 调整特权级别
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = uID;
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		if (!::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
		{
			return FALSE;
		}

		// 关闭访问令牌句柄
		::CloseHandle(hToken);
		hToken = NULL;

		return TRUE;
	}

	static BOOL ListProcessModules(CString &strInfo, DWORD dwPID)
	{
		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		MODULEENTRY32 me32;

		// Take a snapshot of all modules in the specified process.
		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
		if (hModuleSnap == INVALID_HANDLE_VALUE)
		{
			ShowMsgs(_T("提示"), _T("CreateToolhelp32Snapshot (of modules)! Error:%s"), ParseError(GetLastError()));
			return(FALSE);
		}

		// Set the size of the structure before using it.
		me32.dwSize = sizeof(MODULEENTRY32);

		// Retrieve information about the first module,
		// and exit if unsuccessful
		if (!Module32First(hModuleSnap, &me32))
		{
			ShowMsgs(_T("Module32First! Error:%s"), ParseError(GetLastError())); // Show cause of failure
			CloseHandle(hModuleSnap);    // Must clean up the
										 //   snapshot object!
			return(FALSE);
		}

		// Now walk the module list of the process,
		// and display information about each module
		do
		{
			strInfo.AppendFormat(_T("\r\n=========================\r\n"));
			strInfo.AppendFormat(_T("MODULE NAME:%s\r\n"), me32.szModule);
			strInfo.AppendFormat(_T("executable= %s\r\n"), WToT(me32.szExePath).c_str());
			strInfo.AppendFormat(_T("process ID= 0x%08X, ref count (g)= 0x%08X, ref count (p)= 0x%08X, base address= 0x%08X, base size= 0x%08X\r\n"), 
				me32.th32ProcessID, me32.GlblcntUsage, me32.ProccntUsage, (DWORD)me32.modBaseAddr, me32.modBaseSize);

		} while (Module32Next(hModuleSnap, &me32));

		CloseHandle(hModuleSnap);
		return(TRUE);
	}
	static BOOL EnumProcess_R3()
	{
		//快照句柄
		HANDLE hSnapshot = NULL; 
		//注意快照hSnapshot 赋值
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);//创建快照
		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		//枚举进程
		PROCESSENTRY32  procEntry32;
		procEntry32.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(hSnapshot, &procEntry32))
		{
			CloseHandle(hSnapshot);
			return FALSE;
		}

		do
		{
			//*********在这里加入你的操作,比如把进程的名称，PID， ParentID,都能从procEntry32获取
			//******* 进程的路径获取请参照下面的函数


		} while (Process32Next(hSnapshot, &procEntry32));

		//关闭句柄
		CloseHandle(hSnapshot);
		hSnapshot = NULL;
	}

	static DWORD RVA2OFFSET(DWORD dwRVA, IMAGE_NT_HEADERS32 * pImageNTHeaders)
	{
		DWORD dwFileOffset = 0;
		DWORD dwIndex = 0;
		IMAGE_SECTION_HEADER * pImageSectionHeader = 0;
		DWORD dwCount = pImageNTHeaders->FileHeader.NumberOfSections;
		
		for (dwIndex = 0; dwIndex < dwCount; dwIndex++)
		{
			pImageSectionHeader = &IMAGE_FIRST_SECTION(pImageNTHeaders)[dwIndex];
			if ((dwRVA >= pImageSectionHeader->VirtualAddress) &&
				(dwRVA <= (pImageSectionHeader->VirtualAddress + pImageSectionHeader->SizeOfRawData)))
			{
				dwFileOffset = dwRVA - pImageSectionHeader->VirtualAddress + pImageSectionHeader->PointerToRawData;
				break;
			}
		}

		return dwFileOffset;
	}
	static DWORD RVA2OFFSET(DWORD dwRVA, IMAGE_NT_HEADERS64 * pImageNTHeaders)
	{
		DWORD dwFileOffset = 0;
		DWORD dwIndex = 0;
		IMAGE_SECTION_HEADER * pImageSectionHeader = 0;
		DWORD dwCount = pImageNTHeaders->FileHeader.NumberOfSections;

		for (dwIndex = 0; dwIndex < dwCount; dwIndex++)
		{
			pImageSectionHeader = &IMAGE_FIRST_SECTION(pImageNTHeaders)[dwIndex];
			if (dwRVA >= pImageSectionHeader->VirtualAddress &&
				dwRVA <= (pImageSectionHeader->VirtualAddress + pImageSectionHeader->SizeOfRawData))
			{
				dwFileOffset = dwRVA - pImageSectionHeader->VirtualAddress + pImageSectionHeader->PointerToRawData;
				break;
			}
		}

		return dwFileOffset;
	}
	static DWORD RVA2Offset(DWORD dwRVA, IMAGE_NT_HEADERS32 * pNTHeaders)
	{
		DWORD                     dwIndex = 0;
		DWORD                     dwSectionNumber = 0;
		DWORD                     dwStart = 0;
		DWORD                     dwEnd = 0;
		DWORD                     dwOffset = 0;
		IMAGE_SECTION_HEADER *    pSectionHeader = 0;

		dwOffset = 0;
		dwIndex = 0;
		dwSectionNumber = pNTHeaders->FileHeader.NumberOfSections;

		while (dwIndex < dwSectionNumber)
		{
			pSectionHeader = (IMAGE_SECTION_HEADER*)((BYTE *)pNTHeaders
				+ sizeof(IMAGE_NT_HEADERS32)
				+ dwIndex * sizeof(IMAGE_SECTION_HEADER));
			dwStart = pSectionHeader->VirtualAddress;
			dwEnd = pSectionHeader->SizeOfRawData + dwStart;
			if (dwRVA <= dwEnd && dwRVA >= dwStart)
			{
				dwOffset = pSectionHeader->PointerToRawData + dwRVA - dwStart;
				break;
			}
			else
			{
				dwIndex++;
				continue;
			}
		}

		return dwOffset;
	}
	static DWORD RVA2Offset(DWORD dwRVA, IMAGE_NT_HEADERS64 * pNTHeaders)
	{
		DWORD                     dwIndex = 0;
		DWORD                     dwSectionNumber = 0;
		DWORD                     dwStart = 0;
		DWORD                     dwEnd = 0;
		DWORD                     dwOffset = 0;
		IMAGE_SECTION_HEADER   *  pSectionHeader = 0;

		dwOffset = 0;
		dwIndex = 0;
		dwSectionNumber = pNTHeaders->FileHeader.NumberOfSections;

		while (dwIndex < dwSectionNumber)
		{
			pSectionHeader = (IMAGE_SECTION_HEADER *)((BYTE *)pNTHeaders
				+ sizeof(IMAGE_NT_HEADERS64)
				+ dwIndex * sizeof(IMAGE_SECTION_HEADER));
			dwStart = pSectionHeader->VirtualAddress;
			dwEnd = pSectionHeader->SizeOfRawData + dwStart;
			if (dwRVA <= dwEnd && dwRVA >= dwStart)
			{
				dwOffset = pSectionHeader->PointerToRawData + dwRVA - dwStart;
				break;
			}
			else
			{
				dwIndex++;
				continue;
			}
		}

		return dwOffset;
	}

	static int LoadPEInfo32(CString & strInfo, const _TCHAR * ptFileName)
	{
		int nResult = 0;
		WORD wIdx = 0;
		DWORD dwIdx = 0;
		HANDLE hFile = 0;
		HANDLE hFileMapping = 0;
		VOID * lpFileBaseAddress = 0;
		IMAGE_DOS_HEADER * pImageDOSHeader = 0;
		IMAGE_NT_HEADERS32 * pImageNTHeader = 0;
		IMAGE_SECTION_HEADER * pImageSectionHeader = 0;
		struct IMAGE_FILE_MACHINE { DWORD i; DWORD k; _TCHAR v[MAXCHAR];}
		IFMT[] =
		{
			{ 0x01, IMAGE_FILE_MACHINE_I386, _T("x86") },
			{ 0x02, IMAGE_FILE_MACHINE_IA64, _T("Intel Itanium x64") },
			{ 0x04, IMAGE_FILE_MACHINE_AMD64, _T("AMD x64") },
		};
		DWORD VALID_IMAGE_FILE_MACHINE = 0x01;
		DWORD IMAGE_NT_HEADERS_SIZE = sizeof(IMAGE_NT_HEADERS32);
		DWORD IMAGE_THUNK_DATA_SIZE = sizeof(IMAGE_THUNK_DATA32);
		DWORD IMAGE_ORDINAL_FLAG_VALUE = IMAGE_ORDINAL_FLAG32;

		hFile = CreateFile(ptFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			strInfo.AppendFormat(_T("Createfile() Error:%s!\r\n"), ParseError(GetLastError()));
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}

		hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hFileMapping == 0)
		{
			strInfo.AppendFormat(_T("CreateFileMapping() Error:%s!\r\n"), ParseError(GetLastError()));
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}

		lpFileBaseAddress = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
		if (lpFileBaseAddress == 0)
		{
			strInfo.AppendFormat(_T("Couldn't map view of file with MapViewOfFile() Error:%s!\r\n"), ParseError(GetLastError()));
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}

		//-----------------------------------------------------------------------------------------------

		//DOS头结构
		pImageDOSHeader = (IMAGE_DOS_HEADER *)lpFileBaseAddress;

		if (pImageDOSHeader->e_magic != 0x5A4D)
		{
			strInfo.AppendFormat(_T("It's not a PE file！\r\n"));
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}

		//NT头结构
		pImageNTHeader = (IMAGE_NT_HEADERS32 *)((BYTE *)pImageDOSHeader + pImageDOSHeader->e_lfanew);

		strInfo.AppendFormat(_T("======[ DOS Header ]======\r\n"));

		strInfo.AppendFormat(_T("Filename: %s\r\n"), ptFileName);

		strInfo.AppendFormat(_T("======[ NT Header Signature ]======\r\n"));

		strInfo.AppendFormat(_T("Signature: 0x%08X\r\n"), pImageNTHeader->Signature);

		strInfo.AppendFormat(_T("======[ NT Header FileHeader ]======\r\n"));

		for (dwIdx = 0; dwIdx < sizeof(IFMT) / sizeof(struct IMAGE_FILE_MACHINE); dwIdx++)
		{
			if (pImageNTHeader->FileHeader.Machine == IFMT[dwIdx].k)
			{
				if (IFMT[dwIdx].i != (IFMT[dwIdx].i & VALID_IMAGE_FILE_MACHINE))
				{
					strInfo.AppendFormat(_T("Machine: %s\r\n"), IFMT[dwIdx].v);
					nResult = (1);
					break;
				}
				else
				{
					strInfo.AppendFormat(_T("Machine: %s\r\n"), IFMT[dwIdx].v);
					nResult = (0);
					break;
				}
			}
		}

		if (nResult != 0)
		{
			strInfo.AppendFormat(_T("Cannot handle Machine: 0x%04X\r\n"), pImageNTHeader->FileHeader.Machine);
			goto __LEAVE_CLEAN__;
		}

		strInfo.AppendFormat(_T("NumberOfSections: %d\r\n"), pImageNTHeader->FileHeader.NumberOfSections); //文件节的个数
		strInfo.AppendFormat(_T("SizeOfOptionalHeader: %d\r\n"), pImageNTHeader->FileHeader.SizeOfOptionalHeader); //OptionalHeader结构的大小
		strInfo.AppendFormat(_T("Characteristics: 0x%04X\r\n"), pImageNTHeader->FileHeader.Characteristics);
		/*
		0x0001 檔案中沒有重定位（relocation）
		0x0002 檔案是個可執行檔（也就是說不是 OBJ 或 LIB）
		0x2000 檔案是動態聯結函式庫，不是程式。
		*/

		strInfo.AppendFormat(_T("======[ NT Header OptionalHeader ]======\r\n"));

		//程序在载入时的载入点
		strInfo.AppendFormat(_T("ImageBase: 0x%08x\r\n"), pImageNTHeader->OptionalHeader.ImageBase);
		//判断程序的子系统
		strInfo.AppendFormat(_T("Subsystem: 0x%04X\r\n"), pImageNTHeader->OptionalHeader.Subsystem);
		//
		//NATIVE=1 不需要子系統（例如驅動程式）
		//WINDOWS_GUI=2 在 Windows GUI 子系統中執行
		//WINDOWS_CUI=3 在 Windows 字元模式子系統中執行（也就是 console 應用程式）
		//OS2_CUI=5 在 OS/2 字元模式子系統中執行（也就是 OS/2 1.x 應用程式）
		//POSIZ_CUI=7 在 Posix 字元模式子系統中執行
		//
		strInfo.AppendFormat(_T("NumberOfRvaAndSizes: %ld\r\n"), pImageNTHeader->OptionalHeader.NumberOfRvaAndSizes);
		//文件颗粒度，既字节对齐
		strInfo.AppendFormat(_T("FileAlignment: %ld\r\n"), pImageNTHeader->OptionalHeader.FileAlignment);

		for (dwIdx = 0; dwIdx < pImageNTHeader->OptionalHeader.NumberOfRvaAndSizes; dwIdx++)
		{
			strInfo.AppendFormat(_T("%02d size: 0x%08X var: 0x%08X\r\n"),
				dwIdx, pImageNTHeader->OptionalHeader.DataDirectory[dwIdx].Size,
				pImageNTHeader->OptionalHeader.DataDirectory[dwIdx].VirtualAddress);
		}

		strInfo.AppendFormat(_T("======[ Sections Table ]======\r\n"));

		//节的个数
		WORD SectionsNumber = pImageNTHeader->FileHeader.NumberOfSections;

		strInfo.AppendFormat(_T("NumberOfSections: %d\r\n\r\n"), SectionsNumber);
		for (wIdx = 0; wIdx < SectionsNumber; wIdx++)
		{
			pImageSectionHeader = (IMAGE_SECTION_HEADER *)((CHAR *)pImageNTHeader + IMAGE_NT_HEADERS_SIZE + wIdx * sizeof(IMAGE_SECTION_HEADER));
			strInfo.AppendFormat(_T("Name: %s\r\n"), AToT((CHAR *)pImageSectionHeader->Name).c_str());
			//本节RVA地址
			strInfo.AppendFormat(_T("VirtualAddress: 0x%08X\r\n"), pImageSectionHeader->VirtualAddress);
			//文件偏移
			strInfo.AppendFormat(_T("PointerToRawData: 0x%08X\r\n"), pImageSectionHeader->PointerToRawData);
			//本节的大小
			strInfo.AppendFormat(_T("SizeOfRawData: 0x%08X\r\n"), pImageSectionHeader->SizeOfRawData);
			strInfo.AppendFormat(_T("PhysicalAddress: 0x%08X\r\n"), pImageSectionHeader->Misc.PhysicalAddress);
			strInfo.AppendFormat(_T("VirtualSize: 0x%08X\r\n"), pImageSectionHeader->Misc.VirtualSize);
			strInfo.AppendFormat(_T("Characteristics: 0x%08X\r\n"), pImageSectionHeader->Characteristics);
			strInfo.AppendFormat(_T("PointerToRelocations: 0x%08X\r\n\r\n"), pImageSectionHeader->PointerToRelocations);
		}

		//---------------------------------------------------------------------
		DWORD                     dwThunk = 0;
		DWORD                     dwRVA = 0;
		DWORD                     dwOffset = 0;
		IMAGE_IMPORT_DESCRIPTOR * pImportDescriptor = 0;
		BYTE/*IMAGE_THUNK_DATA*/* pThunkData = 0;
		IMAGE_IMPORT_BY_NAME    * pImportByName = 0;

		strInfo.AppendFormat(_T("======[ Import Table ]======\r\n"));

		dwOffset = 0;
		pImportDescriptor = NULL;
		dwRVA = pImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

		dwOffset = RVA2Offset(dwRVA, pImageNTHeader);
		if (dwOffset == 0)
		{
			strInfo.AppendFormat(_T("Can't get the Offset for Import Descriptor\r\n"));
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}
		else
		{
			pImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR *)((CHAR *)pImageDOSHeader + dwOffset);
		}
		//这个dwOffset其实就是文件偏移

		strInfo.AppendFormat(_T("dwOffset: 0x%08X\r\n"), dwOffset);

		strInfo.AppendFormat(_T("IMAGE_THUNK_DATA: %ld\r\n"), IMAGE_THUNK_DATA_SIZE);

		while (pImportDescriptor->Characteristics
			|| pImportDescriptor->TimeDateStamp
			|| pImportDescriptor->ForwarderChain
			|| pImportDescriptor->FirstThunk
			|| pImportDescriptor->Name)
		{
			dwRVA = pImportDescriptor->Name;
			dwOffset = RVA2Offset(dwRVA, pImageNTHeader);
			if (dwOffset == 0)
			{
				strInfo.AppendFormat(_T("Can't get the offset for Name\r\n"));
				nResult = (-1);
				goto __LEAVE_CLEAN__;
			}
			else
			{
				strInfo.AppendFormat(_T("\r\n** %s **\r\n"), AToT((CHAR *)lpFileBaseAddress + dwOffset).c_str());
				strInfo.AppendFormat(_T("dwOffset: 0x%08X\r\n\r\n"), dwOffset);
			}

			strInfo.AppendFormat(_T("FirstThunk: 0x%08X OrigianlFirstThunk: 0x%08X Characteristics: 0x%08X\r\n"),
				pImportDescriptor->FirstThunk,
				pImportDescriptor->OriginalFirstThunk,
				pImportDescriptor->Characteristics);

			strInfo.AppendFormat(_T("FirstThunk: 0x%08X OriginalFirstThunk: 0x%08X Characteristics: 0x%08X\r\n"),
				RVA2Offset(pImportDescriptor->FirstThunk, pImageNTHeader),
				RVA2Offset(pImportDescriptor->OriginalFirstThunk, pImageNTHeader),
				RVA2Offset(pImportDescriptor->Characteristics, pImageNTHeader));

			/*
			这个地方是个有意思的地方，FirstThunk 与 OriginalFirstThunk指向两个不同的地方。
			但这两个地方的内容是一样的。
			*/
			dwThunk = (DWORD)pImportDescriptor->OriginalFirstThunk;
			if (dwThunk == 0)
			{
				dwThunk = (DWORD)pImportDescriptor->FirstThunk;
			}

			while (TRUE) //遍历了IMAGE_THUNK_DATA
			{
				dwRVA = dwThunk;
				dwOffset = RVA2Offset(dwRVA, pImageNTHeader);
				pThunkData = (BYTE *)((CHAR *)pImageDOSHeader + dwOffset + IMAGE_THUNK_DATA_SIZE);
				//每个IMAGE_THUNK_DATA 都是一个RVA地址，指向PIMAGE_IMPORT_BY_NAME
				if (IMAGE_ORDINAL_FLAG_VALUE & (DWORD)pThunkData)
				{
					strInfo.AppendFormat(_T("Ordinal: 0x%04X Name: \r\n"), (WORD)pThunkData);
				}
				else
				{
					dwRVA = *(DWORD *)pThunkData;
					dwOffset = RVA2Offset(dwRVA, pImageNTHeader);
					if (dwOffset == 0)
					{
						break;
					}
					pImportByName = (IMAGE_IMPORT_BY_NAME *)((CHAR *)pImageDOSHeader + dwOffset);

					strInfo.AppendFormat(_T("Ordinal: 0x%04X Name: %s\r\n"), pImportByName->Hint, AToT((CHAR *)pImportByName->Name).c_str());
				}
				dwThunk += IMAGE_THUNK_DATA_SIZE;
			}
			pImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR *)((CHAR *)pImportDescriptor + sizeof(IMAGE_IMPORT_DESCRIPTOR));
		}
		/////////////////////////////////////////////////////////////////////////////////
		//	Export Table
		//

		strInfo.AppendFormat(_T("\r\n"));
		strInfo.AppendFormat(_T("======[ Export Table ]======\r\n"));
		strInfo.AppendFormat(_T("\r\n"));

		DWORD dwExportFileOffset = RVA2Offset(pImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, pImageNTHeader);
		IMAGE_EXPORT_DIRECTORY * pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PUCHAR)lpFileBaseAddress + dwExportFileOffset);
		DWORD dwCount = pImageExportDirectory->NumberOfFunctions;
		DWORD dwOrdinals = RVA2Offset(pImageExportDirectory->AddressOfNameOrdinals, pImageNTHeader);
		DWORD dwFunctions = RVA2Offset(pImageExportDirectory->AddressOfFunctions, pImageNTHeader);
		DWORD dwNames = RVA2Offset(pImageExportDirectory->AddressOfNames, pImageNTHeader);

		strInfo.AppendFormat(_T("NumberOfFunctions: %ld\r\n"), dwCount);

		for (dwIdx = 0; dwIdx < dwCount; dwIdx++)
		{
			WORD * pwOrdinal = (PWORD)((PUCHAR)lpFileBaseAddress + dwOrdinals + dwIdx * sizeof(WORD)); // 地址
			DWORD * pdwFunction = (PDWORD)((PUCHAR)lpFileBaseAddress + dwFunctions + dwIdx * sizeof(DWORD)); // 地址
			DWORD * pdwName = (PDWORD)((PUCHAR)lpFileBaseAddress + dwNames + dwIdx * sizeof(DWORD)); //地址
			CHAR * pszFunctionName = (CHAR *)((PUCHAR)lpFileBaseAddress + RVA2Offset(*pdwName, pImageNTHeader));

			strInfo.AppendFormat(_T("Ordinal: 0x%04X Address: 0x%08X Name: "), *pwOrdinal, *pdwFunction);
			if (dwIdx == *pwOrdinal)
			{
				strInfo.AppendFormat(_T("%s"), AToT(pszFunctionName).c_str());
			}
			strInfo.AppendFormat(_T("\r\n"));
		}
		//-----------------------------------------------------------------------------------

	__LEAVE_CLEAN__:

		if (lpFileBaseAddress)
		{
			UnmapViewOfFile(lpFileBaseAddress); lpFileBaseAddress = NULL;
		}
		if (hFileMapping)
		{
			CloseHandle(hFileMapping); hFileMapping = NULL;
		}
		if (hFile)
		{
			CloseHandle(hFile); hFile = NULL;
		}

		return nResult;
	}

	static int LoadPEInfo64(CString & strInfo, const _TCHAR * ptFileName)
	{
		int nResult = 0;
		WORD wIdx = 0;
		DWORD dwIdx = 0;
		HANDLE hFile = 0;
		HANDLE hFileMapping = 0;
		VOID * lpFileBaseAddress = 0;
		IMAGE_DOS_HEADER * pImageDOSHeader = 0;
		IMAGE_NT_HEADERS64 * pImageNTHeader = 0;
		IMAGE_SECTION_HEADER * pImageSectionHeader = 0;
		struct IMAGE_FILE_MACHINE { DWORD i; DWORD k; _TCHAR v[MAXCHAR];} 
		IFMT[] = {
			{ 0x01, IMAGE_FILE_MACHINE_I386, _T("x86")},
			{ 0x02, IMAGE_FILE_MACHINE_IA64, _T("Intel Itanium x64") },
			{ 0x04, IMAGE_FILE_MACHINE_AMD64, _T("AMD x64") },
		};

		DWORD VALID_IMAGE_FILE_MACHINE = (0x02 | 0x04);
		DWORD IMAGE_NT_HEADERS_SIZE = sizeof(IMAGE_NT_HEADERS64);
		DWORD IMAGE_THUNK_DATA_SIZE = sizeof(IMAGE_THUNK_DATA64);
		ULONGLONG IMAGE_ORDINAL_FLAG_VALUE = IMAGE_ORDINAL_FLAG64;

		hFile = CreateFile(ptFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			strInfo.AppendFormat(_T("Createfile() Error(%ld)!\r\n"), GetLastError());
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}

		hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hFileMapping == 0)
		{
			strInfo.AppendFormat(_T("CreateFileMapping() Error(%ld)!\r\n"), GetLastError());
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}

		lpFileBaseAddress = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
		if (lpFileBaseAddress == 0)
		{
			strInfo.AppendFormat(_T("Couldn't map view of file with MapViewOfFile()！Error(%ld)\r\n"), GetLastError());
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}

		//-----------------------------------------------------------------------------------------------

		//DOS头结构
		pImageDOSHeader = (IMAGE_DOS_HEADER *)lpFileBaseAddress;

		if (pImageDOSHeader->e_magic != 0x5A4D)
		{
			strInfo.AppendFormat(_T("It's not a PE file！\r\n"));
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}
				
		//NT头结构
		pImageNTHeader = (IMAGE_NT_HEADERS64 *)((BYTE *)pImageDOSHeader + pImageDOSHeader->e_lfanew);

		strInfo.AppendFormat(_T("======[ DOS Header ]======\r\n"));

		strInfo.AppendFormat(_T("Filename: %s\r\n"), ptFileName);

		strInfo.AppendFormat(_T("======[ NT Header Signature ]======\r\n"));

		strInfo.AppendFormat(_T("Signature: 0x%08X\r\n"), pImageNTHeader->Signature);

		strInfo.AppendFormat(_T("======[ NT Header FileHeader ]======\r\n"));

		for (dwIdx = 0; dwIdx < sizeof(IFMT) / sizeof(struct IMAGE_FILE_MACHINE); dwIdx++)
		{
			if (pImageNTHeader->FileHeader.Machine == IFMT[dwIdx].k)
			{
				if (IFMT[dwIdx].i != (IFMT[dwIdx].i & VALID_IMAGE_FILE_MACHINE))
				{
					strInfo.AppendFormat(_T("Machine: %s\r\n"), IFMT[dwIdx].v);
					nResult = (1);
					break;
				}
				else
				{
					strInfo.AppendFormat(_T("Machine: %s\r\n"), IFMT[dwIdx].v);
					nResult = (0);
					break;
				}
			}
		}

		if (nResult != 0)
		{
			strInfo.AppendFormat(_T("Cannot handle Machine: 0x%04X\r\n"), pImageNTHeader->FileHeader.Machine);
			goto __LEAVE_CLEAN__;
		}

		strInfo.AppendFormat(_T("NumberOfSections: %d\r\n"), pImageNTHeader->FileHeader.NumberOfSections); //文件节的个数
		strInfo.AppendFormat(_T("SizeOfOptionalHeader: %d\r\n"), pImageNTHeader->FileHeader.SizeOfOptionalHeader); //OptionalHeader结构的大小
		strInfo.AppendFormat(_T("Characteristics: 0x%04X\r\n"), pImageNTHeader->FileHeader.Characteristics);
		/*
		0x0001 檔案中沒有重定位（relocation）
		0x0002 檔案是個可執行檔（也就是說不是 OBJ 或 LIB）
		0x2000 檔案是動態聯結函式庫，不是程式。
		*/

		strInfo.AppendFormat(_T("======[ NT Header OptionalHeader ]======\r\n"));

		//程序在载入时的载入点
		strInfo.AppendFormat(_T("ImageBase: 0x%08x\r\n"), pImageNTHeader->OptionalHeader.ImageBase);
		//判断程序的子系统
		strInfo.AppendFormat(_T("Subsystem: 0x%04X\r\n"), pImageNTHeader->OptionalHeader.Subsystem);
		//
		//NATIVE=1 不需要子系統（例如驅動程式）
		//WINDOWS_GUI=2 在 Windows GUI 子系統中執行
		//WINDOWS_CUI=3 在 Windows 字元模式子系統中執行（也就是 console 應用程式）
		//OS2_CUI=5 在 OS/2 字元模式子系統中執行（也就是 OS/2 1.x 應用程式）
		//POSIZ_CUI=7 在 Posix 字元模式子系統中執行
		//
		strInfo.AppendFormat(_T("NumberOfRvaAndSizes: %ld\r\n"), pImageNTHeader->OptionalHeader.NumberOfRvaAndSizes);
		//文件颗粒度，既字节对齐
		strInfo.AppendFormat(_T("FileAlignment: %ld\r\n"), pImageNTHeader->OptionalHeader.FileAlignment);

		for (dwIdx = 0; dwIdx < pImageNTHeader->OptionalHeader.NumberOfRvaAndSizes; dwIdx++)
		{
			strInfo.AppendFormat(_T("%02d size: 0x%08X var: 0x%08X\r\n"),
				dwIdx, pImageNTHeader->OptionalHeader.DataDirectory[dwIdx].Size,
				pImageNTHeader->OptionalHeader.DataDirectory[dwIdx].VirtualAddress);
		}

		strInfo.AppendFormat(_T("======[ Sections Table ]======\r\n"));

		//节的个数
		WORD SectionsNumber = pImageNTHeader->FileHeader.NumberOfSections;

		strInfo.AppendFormat(_T("NumberOfSections: %d\r\n\r\n"), SectionsNumber);
		for (wIdx = 0; wIdx < SectionsNumber; wIdx++)
		{
			pImageSectionHeader = (IMAGE_SECTION_HEADER *)((CHAR *)pImageNTHeader + IMAGE_NT_HEADERS_SIZE + wIdx * sizeof(IMAGE_SECTION_HEADER));
			strInfo.AppendFormat(_T("Name: %s\r\n"), AToT((CHAR *)pImageSectionHeader->Name).c_str());
			//本节RVA地址
			strInfo.AppendFormat(_T("VirtualAddress: 0x%08X\r\n"), pImageSectionHeader->VirtualAddress);
			//文件偏移
			strInfo.AppendFormat(_T("PointerToRawData: 0x%08X\r\n"), pImageSectionHeader->PointerToRawData);
			//本节的大小
			strInfo.AppendFormat(_T("SizeOfRawData: 0x%08X\r\n"), pImageSectionHeader->SizeOfRawData);
			strInfo.AppendFormat(_T("PhysicalAddress: 0x%08X\r\n"), pImageSectionHeader->Misc.PhysicalAddress);
			strInfo.AppendFormat(_T("VirtualSize: 0x%08X\r\n"), pImageSectionHeader->Misc.VirtualSize);
			strInfo.AppendFormat(_T("Characteristics: 0x%08X\r\n"), pImageSectionHeader->Characteristics);
			strInfo.AppendFormat(_T("PointerToRelocations: 0x%08X\r\n\r\n"), pImageSectionHeader->PointerToRelocations);
		}

		//---------------------------------------------------------------------
		DWORD                     dwThunk = 0;
		DWORD                     dwRVA = 0;
		DWORD                     dwOffset = 0;
		IMAGE_IMPORT_DESCRIPTOR * pImportDescriptor = 0;
		BYTE/*IMAGE_THUNK_DATA*/* pThunkData = 0;
		IMAGE_IMPORT_BY_NAME    * pImportByName = 0;

		strInfo.AppendFormat(_T("======[ Import Table ]======\r\n"));

		dwOffset = 0;
		pImportDescriptor = NULL;
		dwRVA = pImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

		dwOffset = RVA2Offset(dwRVA, pImageNTHeader);
		if (dwOffset == 0)
		{
			strInfo.AppendFormat(_T("Can't get the Offset for Import Descriptor\r\n"));
			nResult = (-1);
			goto __LEAVE_CLEAN__;
		}
		else
		{
			pImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR *)((CHAR *)pImageDOSHeader + dwOffset);
		}
		//这个dwOffset其实就是文件偏移

		strInfo.AppendFormat(_T("dwOffset: 0x%08X\r\n"), dwOffset);

		strInfo.AppendFormat(_T("IMAGE_THUNK_DATA: %ld\r\n"), IMAGE_THUNK_DATA_SIZE);

		while (pImportDescriptor->Characteristics
			|| pImportDescriptor->TimeDateStamp
			|| pImportDescriptor->ForwarderChain
			|| pImportDescriptor->FirstThunk
			|| pImportDescriptor->Name)
		{
			dwRVA = pImportDescriptor->Name;
			dwOffset = RVA2Offset(dwRVA, pImageNTHeader);
			if (dwOffset == 0)
			{
				strInfo.AppendFormat(_T("Can't get the offset for Name\r\n"));
				nResult = (-1);
				goto __LEAVE_CLEAN__;
			}
			else
			{
				strInfo.AppendFormat(_T("\r\n** %s **\r\n"), AToT((CHAR *)lpFileBaseAddress + dwOffset).c_str());
				strInfo.AppendFormat(_T("dwOffset: 0x%08X\r\n\r\n"), dwOffset);
			}

			strInfo.AppendFormat(_T("FirstThunk: 0x%08X OrigianlFirstThunk: 0x%08X Characteristics: 0x%08X\r\n"),
				pImportDescriptor->FirstThunk,
				pImportDescriptor->OriginalFirstThunk,
				pImportDescriptor->Characteristics);

			strInfo.AppendFormat(_T("FirstThunk: 0x%08X OriginalFirstThunk: 0x%08X Characteristics: 0x%08X\r\n"),
				RVA2Offset(pImportDescriptor->FirstThunk, pImageNTHeader),
				RVA2Offset(pImportDescriptor->OriginalFirstThunk, pImageNTHeader),
				RVA2Offset(pImportDescriptor->Characteristics, pImageNTHeader));

			/*
			这个地方是个有意思的地方，FirstThunk 与 OriginalFirstThunk指向两个不同的地方。
			但这两个地方的内容是一样的。
			*/
			dwThunk = (DWORD)pImportDescriptor->OriginalFirstThunk;
			if (dwThunk == 0)
			{
				dwThunk = (DWORD)pImportDescriptor->FirstThunk;
			}

			while (TRUE) //遍历了IMAGE_THUNK_DATA
			{
				dwRVA = dwThunk;
				dwOffset = RVA2Offset(dwRVA, pImageNTHeader);
				pThunkData = (BYTE *)((CHAR *)pImageDOSHeader + dwOffset + IMAGE_THUNK_DATA_SIZE);
				//每个IMAGE_THUNK_DATA 都是一个RVA地址，指向PIMAGE_IMPORT_BY_NAME
				if (IMAGE_ORDINAL_FLAG_VALUE & (DWORD)pThunkData)
				{
					strInfo.AppendFormat(_T("Ordinal: 0x%04X Name: \r\n"), (WORD)pThunkData);
				}
				else
				{
					dwRVA = *(DWORD *)pThunkData;
					dwOffset = RVA2Offset(dwRVA, pImageNTHeader);
					if (dwOffset == 0)
					{
						break;
					}
					pImportByName = (IMAGE_IMPORT_BY_NAME *)((CHAR *)pImageDOSHeader + dwOffset);

					strInfo.AppendFormat(_T("Ordinal: 0x%04X Name: %s\r\n"), pImportByName->Hint, AToT((CHAR *)pImportByName->Name).c_str());
				}
				dwThunk += IMAGE_THUNK_DATA_SIZE;
			}
			pImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR *)((CHAR *)pImportDescriptor + sizeof(IMAGE_IMPORT_DESCRIPTOR));
		}
		/////////////////////////////////////////////////////////////////////////////////
		//	Export Table
		//

		strInfo.AppendFormat(_T("\r\n"));
		strInfo.AppendFormat(_T("======[ Export Table ]======\r\n"));
		strInfo.AppendFormat(_T("\r\n"));

		DWORD dwExportFileOffset = RVA2Offset(pImageNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, pImageNTHeader);
		IMAGE_EXPORT_DIRECTORY * pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PUCHAR)lpFileBaseAddress + dwExportFileOffset);
		DWORD dwCount = pImageExportDirectory->NumberOfFunctions;
		DWORD dwOrdinals = RVA2Offset(pImageExportDirectory->AddressOfNameOrdinals, pImageNTHeader);
		DWORD dwFunctions = RVA2Offset(pImageExportDirectory->AddressOfFunctions, pImageNTHeader);
		DWORD dwNames = RVA2Offset(pImageExportDirectory->AddressOfNames, pImageNTHeader);

		strInfo.AppendFormat(_T("NumberOfFunctions: %ld\r\n"), dwCount);

		for (dwIdx = 0; dwIdx < dwCount; dwIdx++)
		{
			WORD * pwOrdinal = (PWORD)((PUCHAR)lpFileBaseAddress + dwOrdinals + dwIdx * sizeof(WORD)); // 地址
			DWORD * pdwFunction = (PDWORD)((PUCHAR)lpFileBaseAddress + dwFunctions + dwIdx * sizeof(DWORD)); // 地址
			DWORD * pdwName = (PDWORD)((PUCHAR)lpFileBaseAddress + dwNames + dwIdx * sizeof(DWORD)); //地址
			CHAR * pszFunctionName = (CHAR *)((PUCHAR)lpFileBaseAddress + RVA2Offset(*pdwName, pImageNTHeader));

			strInfo.AppendFormat(_T("Ordinal: 0x%04X Address: 0x%08X Name: "), *pwOrdinal, *pdwFunction);
			if (dwIdx == *pwOrdinal)
			{
				strInfo.AppendFormat(_T("%s"), AToT(pszFunctionName).c_str());
			}
			strInfo.AppendFormat(_T("\r\n"));
		}
		//-----------------------------------------------------------------------------------

	__LEAVE_CLEAN__:

		if (lpFileBaseAddress)
		{
			UnmapViewOfFile(lpFileBaseAddress); lpFileBaseAddress = NULL;
		}
		if (hFileMapping)
		{
			CloseHandle(hFileMapping); hFileMapping = NULL;
		}
		if (hFile)
		{
			CloseHandle(hFile); hFile = NULL;
		}

		return nResult;
	}

	static int LoadPEInfo(CString & strInfo, const _TCHAR * ptFileName)
	{
		int nResult = (-1);

		nResult = CProcessManager::LoadPEInfo32(strInfo, ptFileName);
		switch (nResult)
		{
		case 0:	break;
		case 1: strInfo = _T(""); nResult = CProcessManager::LoadPEInfo64(strInfo, ptFileName); break;
		default: break;
		}

		return nResult;
	}
	//根据进程ID终止进程
	static void TerminateProcessByProcessId(DWORD dwProcessId)
	{
		DWORD dwExitCode = 0;
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
		if (hProcess)
		{
			GetExitCodeProcess(hProcess, &dwExitCode);
			TerminateProcess(hProcess, dwExitCode);
			CloseHandle(hProcess);
			hProcess = 0;
		}
	}
	
	//获取进程用户函数：
	static BOOL GetProcessUserName(CString & strUser, DWORD dwID) // 进程ID 
	{
		HANDLE hToken = NULL;
		BOOL bResult = FALSE;
		DWORD dwSize = 0;

		TCHAR szUserName[256] = { 0 };
		TCHAR szDomain[256] = { 0 };
		DWORD dwDomainSize = 256;
		DWORD dwNameSize = 256;

		SID_NAME_USE    SNU;
		PTOKEN_USER pTokenUser = NULL;

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwID);
		
		strUser = _T("SYSTEM");
		if (hProcess == NULL)
		{
			switch (GetLastError())
			{
			case ERROR_ACCESS_DENIED:
				strUser = _T("SYSTEM");
				break;
			default:
				strUser = _T("SYSTEM");
				break;
			}
			return bResult;
		}
		__try
		{
			if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
			{
				bResult = FALSE;
				__leave;
			}

			if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
			{
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
				{
					bResult = FALSE;
					__leave;
				}
			}

			pTokenUser = NULL;
			pTokenUser = (PTOKEN_USER)malloc(dwSize);
			if (pTokenUser == NULL)
			{
				bResult = FALSE;
				__leave;
			}

			if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
			{
				bResult = FALSE;
				__leave;
			}

			if (LookupAccountSid(NULL, pTokenUser->User.Sid, szUserName, &dwNameSize, szDomain, &dwDomainSize, &SNU) != 0)
			{
				strUser = szUserName;
				return TRUE;
			}
		}
		__finally
		{
			if (pTokenUser != NULL)
				free(pTokenUser);
		}

		return FALSE;
	}
	
	static HRESULT GetSystemHardwareInfoSync(WSTRINGVECTORMAP & wvmap, const _TCHAR * ptQueryCommand, const _TCHAR * ptNamespace = _T("root\\CIMV2"), const _TCHAR * ptQueryLanguage = _T("WQL"))
	{
		HRESULT hResult = S_FALSE;
		ULONGLONG uTickTime = 0ULL;
		ULONGLONG uWaitTime = 0ULL;
		//const BSTR bsNamespace = (L"root\\CIMV2");
		CComPtr<IWbemLocator> pIWbemLocator = NULL;
		CComPtr<IWbemServices> pWbemServices = NULL;
		CComPtr<IWbemClassObject> pWbemClassObject = NULL;
		CComPtr<IEnumWbemClassObject> pEnumWbemClassObject = NULL;
		CONST ULONGLONG MILLI_100NANO = 1000000ULL / 100ULL;
		WSTRINGVECTOR wvector;
		BSTR bsPropName;
		SAFEARRAY * psa = NULL;
		ULONG uReturn = 0;
		LONG lObjectCount = 0;
		CString str(_T(""));
		VARIANT varName = { 0 };
		WSTRINGVECTORMAPIT itEnd;
		WSTRINGVECTORMAPIT itIdx;
		LONG lIdx = 0;
		LONG lLbound = 0;
		LONG lUbound = 0;

		uWaitTime = MILLI_100NANO;
		uTickTime = GetCurrentMillisecons();
		
		hResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

		hResult = CoCreateInstance(
			CLSID_WbemAdministrativeLocator,
			NULL,
			CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,
			IID_IUnknown,
			(void **)&pIWbemLocator
		);
		
		if (SUCCEEDED(hResult))
		{
			hResult = pIWbemLocator->ConnectServer(
				(BSTR)(ptNamespace),	// Namespace
				NULL,           // Userid
				NULL,           // PW
				NULL,           // Locale
				0,              // flags
				NULL,           // Authority
				NULL,           // Context
				&pWbemServices
			);
			
			// Free memory resources.
			pIWbemLocator.Release();

			if (FAILED(hResult))
			{
				goto __LEAVE_CLEAN__;
			}

			// Step 5: --------------------------------------------------
			// Set security levels on the proxy -------------------------

			hResult = CoSetProxyBlanket(pWbemServices,                        // Indicates the proxy to set
				RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
				RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
				NULL,                        // Server principal name 
				RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
				RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
				NULL,                        // client identity
				EOAC_NONE);                  // proxy capabilities 

			// Use pWbemServices
			hResult = pWbemServices->ExecQuery((BSTR)(ptQueryLanguage),
				(BSTR)(ptQueryCommand), //BSTR(L"SELECT * FROM Win32_OperatingSystem"),
				WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
				NULL,
				&pEnumWbemClassObject);
			
			if (FAILED(hResult))
			{
				goto __LEAVE_CLEAN__;
			}
			
			while (pEnumWbemClassObject)
			{
				uReturn = 0;
				hResult = pEnumWbemClassObject->Next(WBEM_INFINITE, 1, &pWbemClassObject, &uReturn);
				if (FAILED(hResult) || !uReturn)
				{
					break;
				}
				if (!lObjectCount)
				{
					hResult = pWbemClassObject->GetNames(NULL, WBEM_FLAG_ALWAYS | WBEM_FLAG_NONSYSTEM_ONLY, NULL, &psa);
					if (SUCCEEDED(hResult))
					{
						hResult = SafeArrayGetLBound(psa, 1, &lLbound);
						hResult = SafeArrayGetUBound(psa, 1, &lUbound);
						lObjectCount = lUbound - lLbound + 1;
						for (lIdx = lLbound; lIdx <= lUbound; lIdx++) {
							// Get this property name.
							hResult = SafeArrayGetElement(psa, &lIdx, &bsPropName);
							wvmap.insert(WSTRINGVECTORMAPPAIR(bsPropName, wvector));
							//len = (unsigned)SysStringLen(bsPropName);
							//length[0] = MY_MAX(length[0], len);
						} // enfor i

						hResult = SafeArrayDestroy(psa);
						psa = NULL;
					}
				}
				itEnd = wvmap.end();
				itIdx = wvmap.begin();
				for (; itIdx != itEnd; itIdx++)
				{
					hResult = pWbemClassObject->Get((LPCWSTR)(itIdx->first.c_str()), 0, &varName, 0, 0);
					switch (varName.vt)
					{
					case VT_NULL:
						itIdx->second.push_back((L""));
						break;
					case VT_BSTR:
						itIdx->second.push_back(V_BSTR(&varName));
						break;
					default:
						str = _T("");
						VariantToCString(str, varName);
						itIdx->second.push_back((LPCWSTR)str);
						break;
					}
					if (FAILED(hResult))
					{
						printf("Failed to get the data from the query  Error code = 0x%X\n", hResult);
						//return WBEM_E_FAILED;       // Program has failed.
					}
				}

				pWbemClassObject.Release();
			}
				
			(LONGLONG(GetCurrentMillisecons() - uTickTime - uWaitTime) < 0LL);
		}

	__LEAVE_CLEAN__:
		
		pEnumWbemClassObject.Release();

		// Free memory resources.
		pIWbemLocator.Release();
		
		// Clean up
		pWbemServices.Release();

		CoUninitialize();

		return hResult;
	}
	
	static HRESULT GetSystemHardwareInfoAsync(WSTRINGVECTORMAP & wvmap, const _TCHAR * ptQueryCommand, const _TCHAR * ptNamespace = _T("root\\CIMV2"), const _TCHAR * ptQueryLanguage = _T("WQL"), DWORD dwMilliseconds = 0)
	{
		HRESULT hResult = S_FALSE;
		ULONGLONG uTickTime = 0ULL;
		ULONGLONG uWaitTime = 0ULL;
		//const BSTR bsNamespace = (L"root\\CIMV2");
		CComPtr<IWbemLocator> pIWbemLocator = NULL;
		CComPtr<IWbemServices> pWbemServices = NULL;
		CComPtr<CWbemObjectQuerySink> pResponseSink = NULL;
		CONST ULONGLONG MILLI_100NANO = 1000000ULL / 100ULL;
		
		hResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

		hResult = CoCreateInstance(
			CLSID_WbemAdministrativeLocator,
			NULL,
			CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,
			IID_IUnknown,
			(void **)&pIWbemLocator
			);

		if (SUCCEEDED(hResult))
		{
			hResult = pIWbemLocator->ConnectServer(
				(BSTR)(ptNamespace),	// Namespace
				NULL,           // Userid
				NULL,           // PW
				NULL,           // Locale
				0,              // flags
				NULL,           // Authority
				NULL,           // Context
				&pWbemServices
				);

			// Free memory resources.
			pIWbemLocator.Release();

			if (FAILED(hResult))
			{
				goto __LEAVE_CLEAN__;
			}

			// Step 5: --------------------------------------------------
			// Set security levels on the proxy -------------------------

			hResult = CoSetProxyBlanket(pWbemServices,                        // Indicates the proxy to set
				RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
				RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
				NULL,                        // Server principal name 
				RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
				RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
				NULL,                        // client identity
				EOAC_NONE);                  // proxy capabilities 

			// Use pWbemServices
			pResponseSink = new CWbemObjectQuerySink();

			hResult = pWbemServices->ExecQueryAsync((BSTR)(ptQueryLanguage),
				(BSTR)(ptQueryCommand), //BSTR(L"SELECT * FROM Win32_OperatingSystem"),
				WBEM_FLAG_BIDIRECTIONAL,
				NULL,
				pResponseSink);

			if (FAILED(hResult))
			{
				goto __LEAVE_CLEAN__;
			}

			uWaitTime = dwMilliseconds * MILLI_100NANO;
			uTickTime = GetCurrentMillisecons();
			while (!pResponseSink->m_wvmap.size() && (LONGLONG(GetCurrentMillisecons() - uTickTime - uWaitTime) < 0LL))
			{
				Sleep(sizeof(BYTE));
			}

			pWbemServices->CancelAsyncCall(pResponseSink);
			wvmap.swap(pResponseSink->m_wvmap);
		}

	__LEAVE_CLEAN__:

		// Free memory resources.
		pIWbemLocator.Release();

		// Free memory resources.
		pResponseSink.Release();

		// Clean up
		pWbemServices.Release();

		CoUninitialize();

		return hResult;
	}

	//////////////////////////////////////////////////////////////////////////////
	//判断进程是64位还是32位(bIsWow64为TRUE为64位，返回FALSE为32位)
	static BOOL IsWow64Process(BOOL & bIsWow64, HANDLE hProcess)
	{
		typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
		BOOL bResult = FALSE;
		LPFN_ISWOW64PROCESS fnIsWow64Process = NULL;

		if (hProcess)
		{
			fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("KERNEL32.DLL")), "IsWow64Process");
			if (fnIsWow64Process)
			{
				bResult = fnIsWow64Process(hProcess, &bIsWow64);
			}
		}

		return bResult;
	}
	static BOOL IsWow64Process(BOOL & bIsWow64, DWORD dwProcessId)
	{
		BOOL bResult = FALSE;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
		if (hProcess)
		{
			bResult = IsWow64Process(bIsWow64, hProcess);
			CloseHandle(hProcess);
			hProcess = NULL;
		}
		
		return bResult;
	}
	
	static BOOL IsWow64System()
	{
		typedef VOID(__stdcall*LPFN_GETNATIVESYSTEMINFO)(LPSYSTEM_INFO lpSystemInfo);
		
		BOOL bResult = FALSE;
		SYSTEM_INFO si = { 0 };
		LPFN_GETNATIVESYSTEMINFO fnGetNativeSystemInfo = NULL;

		fnGetNativeSystemInfo = (LPFN_GETNATIVESYSTEMINFO)GetProcAddress(GetModuleHandle(_T("KERNEL32.DLL")), "GetNativeSystemInfo");
		if (fnGetNativeSystemInfo)
		{
			fnGetNativeSystemInfo(&si);

			if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ||
				si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			{
				bResult = TRUE;
			}
		}

		return bResult;
	}

};
