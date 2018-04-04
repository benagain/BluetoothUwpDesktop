#pragma once

#import "../MyidBlueKsp/bin/Debug/MyidBlueKsp.tlb"

#define PROVIDER_MAGIC 0x4D795068      // MyPh

class ProviderHandle
{
public:
	ProviderHandle();

	DWORD				magic;		///< identifying magic number
	std::wstring		name;		///< name of the KSP
	std::wstring		context;	///< context string

	MyidBlueKsp::IBlueKspPtr ksp;		///< Managed KSP

	NCryptKeyName* EnumerateKeys(unsigned long numAlreadySeen);

	MyidBlueKsp::IBlueKeyPtr FindKey(const wchar_t* name);

private:
	SAFEARRAY * enumeratedKeys = nullptr;
};

_Success_(return != NULL) ProviderHandle* ValidateProviderHandle(_In_ NCRYPT_PROV_HANDLE provider);
_Success_(return != NULL) MyidBlueKsp::IBlueKeyPtr ValidateKeyHandle2(_In_ NCRYPT_PROV_HANDLE provider, _In_ NCRYPT_KEY_HANDLE key);

