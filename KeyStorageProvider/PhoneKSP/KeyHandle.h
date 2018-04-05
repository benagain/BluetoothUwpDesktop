#pragma once

#include <atlsafe.h>
#import "../MyidBlueKsp/bin/Debug/MyidBlueKsp.tlb"

class KeyHandle
{
public:
	KeyHandle(MyidBlueKsp::IBlueKeyPtr key);
	~KeyHandle();

	/// Identifying magic number.
	const DWORD magic; 

	const std::wstring name;
	const std::wstring context;

	void ChangeContext(std::wstring && context);

	unsigned long CalculateSignatureSize(const CComSafeArray<BYTE> & bytes, MyidBlueKsp::HashAlgorithm hashAlgorithm);

	MyidBlueKsp::IBlueKeyPtr key;		///< The actual bluetooth key being wrapped.
};

_Success_(return != NULL) KeyHandle* ValidateKeyHandle(_In_ NCRYPT_PROV_HANDLE provider, _In_ NCRYPT_KEY_HANDLE key);


