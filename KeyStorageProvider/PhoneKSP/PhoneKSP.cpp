#include "stdafx.h"

HINSTANCE g_hInstance;		///< DLL instance
//List of keys/providers
//LIST_ENTRY g_SampleKspEnumStateList;


BOOL WINAPI DllMain(_In_ HINSTANCE hInstance, _In_ DWORD dwReason, _In_ LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	g_hInstance = (HINSTANCE)hInstance;

	if(dwReason == DLL_PROCESS_ATTACH)
	{
//		InitializeListHead(&g_SampleKspEnumStateList);
	}
	else if(dwReason == DLL_PROCESS_DETACH)
	{
		//if (g_hRSAProvider)
		//{
		//	BCryptCloseAlgorithmProvider(g_hRSAProvider,0);
		//}
	}
	return TRUE;
}
