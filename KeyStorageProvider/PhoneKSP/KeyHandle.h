#pragma once

#define KEY_MAGIC 0x4D79506B      // MyPk

class KeyHandle
{
public:
	KeyHandle();
	~KeyHandle();

	DWORD				magic;		///< identifying magic number
	std::wstring		name;		///< name of the Key
	std::wstring		context;	///< context string

	BCRYPT_KEY_HANDLE	publicKey;
};

_Success_(return != NULL) KeyHandle* ValidateKeyHandle(_In_ NCRYPT_KEY_HANDLE hKey);

