#include "stdafx.h"
#include "KeyHandle.h"
#include "ProviderHandle.h"

const unsigned long KEY_MAGIC = 0x4D79506B;      // MyPk

KeyHandle::KeyHandle(MyidBlueKsp::IBlueKeyPtr key)
	: magic(KEY_MAGIC)
	, key(key)
{}

KeyHandle::~KeyHandle()
{}

void KeyHandle::ChangeContext(std::wstring &&)
{
	//this->context = context;
}

unsigned long KeyHandle::CalculateSignatureSize(const CComSafeArray<BYTE> & bytes, MyidBlueKsp::HashAlgorithm hashAlgorithm)
{
	return (DWORD)key->CalculateSignatureSize(const_cast<CComSafeArray<BYTE>&&>(bytes), hashAlgorithm);
}



_Success_(return != NULL) KeyHandle* ValidateKeyHandle(_In_ NCRYPT_PROV_HANDLE provider, _In_ NCRYPT_KEY_HANDLE key)
{
	if (ValidateProviderHandle(provider) == nullptr)
		return nullptr;

	if (key == 0)
		return nullptr;

	auto handle = reinterpret_cast<KeyHandle*>(key);
	if (handle->magic != KEY_MAGIC)
		return nullptr;

	return handle;
}
