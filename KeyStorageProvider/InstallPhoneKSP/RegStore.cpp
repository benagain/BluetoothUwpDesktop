#include "stdafx.h"
#include "RegStore.h"
#include "..\Common\PhoneKSP.h"


#pragma comment(lib, "Crypt32.lib")

void RegisterStore(void)
{
	if (!CertRegisterSystemStore(MYID_PHONE_STORE, CERT_SYSTEM_STORE_LOCAL_MACHINE, nullptr, nullptr))
	{
		wprintf(L"Register certificate store failed\n");
	}
}

void UnRegisterStore(void)
{
	if (!CertUnregisterSystemStore(MYID_PHONE_STORE, CERT_SYSTEM_STORE_LOCAL_MACHINE | CERT_STORE_DELETE_FLAG))
	{
		wprintf(L"Unregister certificate store failed\n");
	}
}
