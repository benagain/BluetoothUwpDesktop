#pragma once

#include <memory>
#import "../MyidBlueKsp/bin/Debug/MyidBlueKsp.tlb"

class KeyHandle;
#include "KeyHandle.h"

#define PROVIDER_MAGIC 0x4D795068      // MyPh

class ProviderHandle
{
public:
	ProviderHandle();

	DWORD				magic;		///< identifying magic number
	std::wstring		name;		///< name of the KSP
	std::wstring		context;	///< context string

	MyidBlueKsp::IBlueKspPtr* ksp;		///< Managed KSP

	NCryptKeyName* EnumerateKeys(unsigned long numAlreadySeen);

	std::unique_ptr<KeyHandle> FindKey(const wchar_t* name);
	tl::expected<std::unique_ptr<KeyHandle>, SECURITY_STATUS> FindKey2(const wchar_t* name);

private:
	SAFEARRAY * enumeratedKeys = nullptr;
};

tl::expected<ProviderHandle*, SECURITY_STATUS> ValidateProviderHandle2(_In_ NCRYPT_PROV_HANDLE provider);
_Success_(return != NULL) ProviderHandle* ValidateProviderHandle(_In_ NCRYPT_PROV_HANDLE provider);

