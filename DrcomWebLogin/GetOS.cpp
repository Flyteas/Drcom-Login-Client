/*****************************************************************************
Operating System Version PlatformID
Windows 8 6.2 VER_PLATFORM_WIN32_NT (=2)
Windows 7 6.1 VER_PLATFORM_WIN32_NT
Windows Server 2008 R2 6.1 VER_PLATFORM_WIN32_NT
Windows Server 2008 6.0 VER_PLATFORM_WIN32_NT
Windows Vista 6.0 VER_PLATFORM_WIN32_NT
Windows Server 2003 R2 5.2 VER_PLATFORM_WIN32_NT
Windows Server 2003 5.2 VER_PLATFORM_WIN32_NT
Windows XP 64-Bit Edition 5.2 VER_PLATFORM_WIN32_NT
Windows XP 5.1 VER_PLATFORM_WIN32_NT
Windows 2000 5.0 VER_PLATFORM_WIN32_NT
Windows NT 4.0 4.0 VER_PLATFORM_WIN32_NT
Windows NT 3.51 3.51 ? VER_PLATFORM_WIN32_NT
Windows Millennium Edition 4.90 VER_PLATFORM_WIN32_WINDOWS (=1)
Windows 98 4.10 VER_PLATFORM_WIN32_WINDOWS
Windows 95 4.0 VER_PLATFORM_WIN32_WINDOWS
Windows 3.1 3.1 ? VER_PLATFORM_WIN32s (=0)
*****************************************************************************/

#include "stdafx.h"
#include "GetOS.h"

BOOL GetOSDisplayString(LPTSTR pszOS)
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	PGPI pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;
	ZeroMemory(&si,sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi,sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize =sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx =GetVersionEx((OSVERSIONINFO*)&osvi);
	if(!bOsVersionInfoEx)
		return FALSE;
	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
	pGNSI =(PGNSI)GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")),"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else
		GetSystemInfo(&si);
	if( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && osvi.dwMajorVersion >4)
	{
		StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));
		// Test for the specific product.
		if( osvi.dwMajorVersion >=6)
		{
			if( osvi.dwMajorVersion >6|| osvi.dwMinorVersion >2)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows 8 Later "));// for new OS
			}
			else if( osvi.dwMinorVersion ==0)
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
				else
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 "));
			}
			else if( osvi.dwMinorVersion ==1)
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
				else
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 "));
			}
			else if( osvi.dwMinorVersion ==2)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows 8 "));
			}
			pGPI =(PGPI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"GetProductInfo");
			pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion,0,0,&dwType);
			switch( dwType )
			{
			case PRODUCT_ULTIMATE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition"));
				break;
			case PRODUCT_PROFESSIONAL:
				StringCchCat(pszOS, BUFSIZE, TEXT("Professional"));
				break;
			case PRODUCT_HOME_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition"));
				break;
			case PRODUCT_HOME_BASIC:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition"));
				break;
			case PRODUCT_ENTERPRISE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition"));
				break;
			case PRODUCT_BUSINESS:
				StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition"));
				break;
			case PRODUCT_STARTER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition"));
				break;
			case PRODUCT_CLUSTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition"));
				break;
			case PRODUCT_DATACENTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition"));
				break;
			case PRODUCT_DATACENTER_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)"));
				break;
			case PRODUCT_ENTERPRISE_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition"));
				break;
			case PRODUCT_ENTERPRISE_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)"));
				break;
			case PRODUCT_ENTERPRISE_SERVER_IA64:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems"));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server"));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition"));
				break;
			case PRODUCT_STANDARD_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition"));
				break;
			case PRODUCT_STANDARD_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)"));
				break;
			case PRODUCT_WEB_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition"));
				break;
			}
		}
		else if(osvi.dwMinorVersion ==2)
		{
			if(GetSystemMetrics(SM_SERVERR2))
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003 R2, "));
			else if( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows Storage Server 2003"));
			else if( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows Home Server"));
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP Professional x64 Edition"));
			}
			else
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003, "));
			// Test for the server type.
			if( osvi.wProductType != VER_NT_WORKSTATION )
			{
				if( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition for Itanium-based Systems"));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems"));
				}
				else if( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter x64 Edition"));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise x64 Edition"));
					else
						StringCchCat(pszOS, BUFSIZE, TEXT("Standard x64 Edition"));
				}
				else
				{
					if( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
						StringCchCat(pszOS, BUFSIZE, TEXT("Compute Cluster Edition"));
					else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition"));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition"));
					else if( osvi.wSuiteMask & VER_SUITE_BLADE )
						StringCchCat(pszOS, BUFSIZE, TEXT("Web Edition"));
					else
						StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition"));
				}
			}
		}
		else if(/*osvi.dwMajorVersion == 5 &&*/ osvi.dwMinorVersion ==1)
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
			if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Edition"));
			else 
				StringCchCat(pszOS, BUFSIZE, TEXT("Professional"));
		}
		else if(/*osvi.dwMajorVersion == 5 &&*/ osvi.dwMinorVersion ==0)
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));
			if( osvi.wProductType == VER_NT_WORKSTATION )
			{
				StringCchCat(pszOS, BUFSIZE, TEXT("Professional"));
			}
			else
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Server"));
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					StringCchCat(pszOS, BUFSIZE, TEXT("Advanced Server"));
				else
					StringCchCat(pszOS, BUFSIZE, TEXT("Server"));
			}
		}
		// Include service pack (if any) and build number.
		if( _tcslen(osvi.szCSDVersion)>0)
		{
			StringCchCat(pszOS, BUFSIZE, TEXT(" "));
			StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
		}
		TCHAR buf[80];
		StringCchPrintf( buf,80, TEXT(" (build %d)"), osvi.dwBuildNumber);
		StringCchCat(pszOS, BUFSIZE, buf);
		if( osvi.dwMajorVersion >=6)
		{
			if( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				StringCchCat(pszOS, BUFSIZE, TEXT(", 64-bit"));
			else if(si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL GetOldOSVer(LPSTR pszOldOS)
{
	OSVERSIONINFOEXA osvi;
	BOOL bOsVersionInfoEx;
	ZeroMemory(&osvi,sizeof(OSVERSIONINFOEXA));
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.
	osvi.dwOSVersionInfoSize =sizeof(OSVERSIONINFOEXA);
	bOsVersionInfoEx =GetVersionExA((OSVERSIONINFOA *)&osvi);
	if(!bOsVersionInfoEx )
	{
		osvi.dwOSVersionInfoSize =sizeof(OSVERSIONINFOA);
		if(!GetVersionExA((OSVERSIONINFOA *)&osvi))
		{
			StringCchCatA(pszOldOS, BUFSIZE,"Unknow Windows?");
			return FALSE;
		}
	}
	switch(osvi.dwPlatformId)
	{
		// Test for the Windows NT product family.
	case VER_PLATFORM_WIN32_NT:
		if(osvi.dwMajorVersion <=4)
		{
			StringCchCatA(pszOldOS, BUFSIZE,"Microsoft Windows NT ");

		}
		else
		{
			return FALSE;
		}
		break;
		// Test for the Windows Me/98/95.
	case VER_PLATFORM_WIN32_WINDOWS:
		if(osvi.dwMajorVersion ==4)
		{
			switch(osvi.dwMinorVersion)
			{
			case 0:
				StringCchCatA(pszOldOS, BUFSIZE,"Microsoft Windows 95 ");
				if(osvi.szCSDVersion[1]=='C'|| osvi.szCSDVersion[1]=='B')
					StringCchCatA(pszOldOS, BUFSIZE,"OSR2 ");
				break;
			case 3:
				StringCchCatA(pszOldOS, BUFSIZE,"Microsoft Windows 95 OSR2 ");
			case 10:
				StringCchCatA(pszOldOS, BUFSIZE,"Microsoft Windows 98 ");
				if( osvi.szCSDVersion[1]=='A'|| osvi.szCSDVersion[1]=='B')
					StringCchCatA(pszOldOS, BUFSIZE,"SE ");
				break;
			case 90:
				StringCchCatA(pszOldOS, BUFSIZE,"Microsoft Windows Millennium Edition ");
				break;
			}
		}
		break;
	case VER_PLATFORM_WIN32s:
		StringCchCatA(pszOldOS, BUFSIZE,"Microsoft Win32s ");
		break;
	default:
		return FALSE;
	}
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		// Test for specific product on Windows NT 4.0 SP6 and later.
		if( bOsVersionInfoEx )
		{
			// Test for the workstation type.
			if( osvi.wProductType == VER_NT_WORKSTATION)
			{
				if( osvi.dwMajorVersion ==4)
					StringCchCatA(pszOldOS, BUFSIZE,"Workstation 4.0 ");
				else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
					StringCchCatA(pszOldOS, BUFSIZE,"Home Edition ");
				else
					StringCchCatA(pszOldOS, BUFSIZE,"Professional ");
			}
			// Test for the server type.
			else if( osvi.wProductType == VER_NT_SERVER ||
				osvi.wProductType == VER_NT_DOMAIN_CONTROLLER )
			{
				// Windows NT 4.0
				if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					StringCchCatA(pszOldOS, BUFSIZE,"Server 4.0, Enterprise Edition ");
				else
					StringCchCatA(pszOldOS, BUFSIZE,"Server 4.0 ");
			}
		}
		// Test for specific product on Windows NT 4.0 SP5 and earlier
		else
		{
			HKEY hKey;
			TCHAR szProductType[BUFSIZE];
			DWORD dwBufLen=BUFSIZE*sizeof(TCHAR);
			LONG lRet =RegOpenKeyEx( HKEY_LOCAL_MACHINE,TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),0, KEY_QUERY_VALUE,&hKey );
			if( lRet != ERROR_SUCCESS )
				return FALSE;
			lRet =RegQueryValueEx( hKey, TEXT("ProductType"),
				NULL, NULL,(LPBYTE) szProductType,&dwBufLen);
			RegCloseKey( hKey );
			if((lRet != ERROR_SUCCESS)||(dwBufLen > BUFSIZE*sizeof(TCHAR)))
				return FALSE;
			if( lstrcmpi( TEXT("WINNT"), szProductType)==0)
				StringCchCatA(pszOldOS, BUFSIZE,"Workstation ");
			if( lstrcmpi( TEXT("LANMANNT"), szProductType)==0)
				StringCchCatA(pszOldOS, BUFSIZE,"Server ");
			if( lstrcmpi( TEXT("SERVERNT"), szProductType)==0)
				StringCchCatA(pszOldOS, BUFSIZE,"Advanced Server ");
			char szVer[20];
			StringCchPrintfA(szVer,sizeof(szVer),"%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
			StringCchCatA(pszOldOS, BUFSIZE, szVer);
		}
		// Display service pack (if any) and build number.
		char sp6[]="Service Pack 6";
		char szBuf[40];
		if( osvi.dwMajorVersion ==4&&
			lstrcmpiA( osvi.szCSDVersion, sp6 )==0)
		{
			HKEY hKey;
			LONG lRet;
			// Test for SP6 versus SP6a.
			lRet =RegOpenKeyEx( HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),0, KEY_QUERY_VALUE,&hKey );
			if( lRet == ERROR_SUCCESS )
			{
				StringCchPrintfA(szBuf,sizeof(szBuf),"Service Pack 6a (Build %d)", osvi.dwBuildNumber &0xFFFF);
				StringCchCatA(pszOldOS, BUFSIZE, szBuf);
			}
			else// Windows NT 4.0 prior to SP6a
			{
				StringCchPrintfA(szBuf,sizeof(szBuf),"%s (Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber &0xFFFF);
				StringCchCatA(pszOldOS, BUFSIZE, szBuf);
			}
			RegCloseKey( hKey );
		}
		else// not Windows NT 4.0
		{
			StringCchPrintfA(szBuf,sizeof(szBuf),"%s (Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber &0xFFFF);
			StringCchCatA(pszOldOS, BUFSIZE, szBuf);
		}
	}
	return TRUE;
}