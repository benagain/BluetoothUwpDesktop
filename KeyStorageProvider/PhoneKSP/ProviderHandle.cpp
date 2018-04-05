#include "stdafx.h"
#include "ProviderHandle.h"
#include "KeyHandle.h"

#import "../MyidBlueKsp/bin/Debug/MyidBlueKsp.tlb"


ProviderHandle::ProviderHandle()
	: magic(PROVIDER_MAGIC)
	, ksp(new MyidBlueKsp::IBlueKspPtr(__uuidof(MyidBlueKsp::BlueKsp)))
{}


NCryptKeyName* ProviderHandle::EnumerateKeys(unsigned long numAlreadySeen)
{
	if(enumeratedKeys == nullptr)
		enumeratedKeys = (*ksp)->EnumerateKeys2();

	auto numElements = enumeratedKeys->rgsabound->cElements;
	
	if (numAlreadySeen >= numElements)
		return nullptr;

	MyidBlueKsp::IBlueKey *kkey;
	auto hr = SafeArrayGetElement(enumeratedKeys, (LONG*)&numAlreadySeen, &kkey);
	UNREFERENCED_PARAMETER(hr);

	auto keyName = new NCryptKeyName{};
	kkey->GetCryptName(reinterpret_cast<MyidBlueKsp::CryptKeyName*>(keyName));
	return keyName;
}

std::unique_ptr<KeyHandle> ProviderHandle::FindKey(const wchar_t* searchName)
{
	UNREFERENCED_PARAMETER(searchName);

	if (enumeratedKeys == nullptr)
		enumeratedKeys = (*ksp)->EnumerateKeys2();

	auto numElements = enumeratedKeys->rgsabound->cElements;

	for (auto i = 0u; i < numElements; ++i)
	{
		MyidBlueKsp::IBlueKey *kkey;
		auto hr = SafeArrayGetElement(enumeratedKeys, (LONG*)&i, &kkey);
		UNREFERENCED_PARAMETER(hr);

		auto keyName = kkey->Name;
		if (keyName == _bstr_t(searchName))
			return std::make_unique<KeyHandle>(MyidBlueKsp::IBlueKeyPtr(kkey));
	}

	return {};
}

_Success_(return != NULL) ProviderHandle* ValidateProviderHandle(_In_ NCRYPT_PROV_HANDLE provider)
{
	if (provider == 0)
		return nullptr;

	ProviderHandle* handle = reinterpret_cast<ProviderHandle*>(provider);
	if (handle->magic != PROVIDER_MAGIC)
		return nullptr;

	return handle;
}
