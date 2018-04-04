#include "stdafx.h"
#include "KeyHandle.h"

KeyHandle::KeyHandle()
	: magic(KEY_MAGIC)
	, publicKey(nullptr)
{}

KeyHandle::~KeyHandle()
{}


_Success_(return != NULL) KeyHandle* ValidateKeyHandle(_In_ NCRYPT_KEY_HANDLE hKey)
{
	if (hKey == 0)
		return nullptr;

	KeyHandle* handle = reinterpret_cast<KeyHandle*>(hKey);
	if (handle->magic != KEY_MAGIC)
		return nullptr;

	return handle;
}

