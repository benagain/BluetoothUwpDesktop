#include "stdafx.h"
#include "RegKSP.h"
#include "..\Common\PhoneKSP.h"

#pragma comment(lib, "bcrypt_provider.lib")


void RegisterKSP(void)
{
	static PWSTR algs[] = {NCRYPT_KEY_STORAGE_ALGORITHM};
	static CRYPT_INTERFACE_REG ks = {NCRYPT_KEY_STORAGE_INTERFACE, CRYPT_LOCAL, _countof(algs), algs};
	static PCRYPT_INTERFACE_REG intf[] = {&ks};
	static CRYPT_IMAGE_REG image = {MYID_PHONE_BINARY, _countof(intf), intf};
	static CRYPT_PROVIDER_REG reg = {0, nullptr, &image, nullptr};

	NTSTATUS status = BCryptRegisterProvider(MYID_PHONE_PROVIDER_NAME, CRYPT_OVERWRITE, &reg);
	if (!BCRYPT_SUCCESS(status))
	{
		wprintf(L"BCryptRegisterProvider failed with error code 0x%08x\n", status);
		return;
	}

	//status = BCryptAddContextFunction(CRYPT_LOCAL, CRYPT_DEFAULT_CONTEXT, NCRYPT_KEY_STORAGE_INTERFACE, NCRYPT_KEY_STORAGE_ALGORITHM, CRYPT_PRIORITY_BOTTOM);
	//if (!BCRYPT_SUCCESS(status))
	//{
	//	wprintf(L"BCryptAddContextFunction failed with error code 0x%08x\n", status);
	//}

	status = BCryptAddContextFunctionProvider(CRYPT_LOCAL, CRYPT_DEFAULT_CONTEXT, NCRYPT_KEY_STORAGE_INTERFACE, NCRYPT_KEY_STORAGE_ALGORITHM, MYID_PHONE_PROVIDER_NAME, CRYPT_PRIORITY_BOTTOM);
	if (!BCRYPT_SUCCESS(status))
	{
		wprintf(L"BCryptAddContextFunctionProvider failed with error code 0x%08x\n", status);
	}
}

void UnRegisterKSP(void)
{
	NTSTATUS status = BCryptRemoveContextFunctionProvider(CRYPT_LOCAL, CRYPT_DEFAULT_CONTEXT, NCRYPT_KEY_STORAGE_INTERFACE, NCRYPT_KEY_STORAGE_ALGORITHM, MYID_PHONE_PROVIDER_NAME);
	if (!BCRYPT_SUCCESS(status))
	{
		wprintf(L"BCryptRemoveContextFunctionProvider failed with error code 0x%08x\n", status);
	}

	status = BCryptUnregisterProvider(MYID_PHONE_PROVIDER_NAME);
	if (!BCRYPT_SUCCESS(status))
	{
		wprintf(L"BCryptUnregisterProvider failed with error code 0x%08x\n", status);
	}
}
