#include "stdafx.h"
#include "KSPInterface.h"
#include "ProviderHandle.h"
#include "KeyHandle.h"
#include "Helper.h"
#include <atlsafe.h>
#include <vector>

#pragma comment(lib, "bcrypt.lib")

CComSafeArray<BYTE> TakeControl(SAFEARRAY* array);
CComSafeArray<BYTE> BufferToByteArray(unsigned long size, BYTE* buffer);
bool ByteArrayIntoBuffer(const CComSafeArray<BYTE> & array, unsigned long size, BYTE* buffer);

NCRYPT_KEY_STORAGE_FUNCTION_TABLE KSPFunctionTable =
{
	NCRYPT_KEY_STORAGE_INTERFACE_VERSION_2,
	OpenStorageProvider,
	OpenKey,
	CreatePersistedKey,
	GetProviderProperty,
	GetKeyProperty,
	SetProviderProperty,
	SetKeyProperty,
	FinalizeKey,
	DeleteKey,
	FreeProvider,
	FreeKey,
	FreeBuffer,
	Encrypt,
	Decrypt,
	IsAlgSupported,
	EnumAlgorithms,
	EnumKeys,
	ImportKey,
	ExportKey,
	SignHash,
	VerifySignature,
	PromptUser,
	NotifyChangeKey,
	SecretAgreement,
	DeriveKey,
	FreeSecret,
	KeyDerivation
};

/// Get the key storage interface function table
_Check_return_ NTSTATUS WINAPI GetKeyStorageInterface(
	_In_   LPCWSTR pszProviderName,									///< provider name, not used
	_Out_  NCRYPT_KEY_STORAGE_FUNCTION_TABLE **ppFunctionTable,		///< function dispatch table
	_In_   DWORD dwFlags)											///< not used
{
	UNREFERENCED_PARAMETER(pszProviderName);
	UNREFERENCED_PARAMETER(dwFlags);

	*ppFunctionTable = &KSPFunctionTable;

	return ERROR_SUCCESS;
}

/// Load and initialize the provider
_Check_return_ KSP_STATUS WINAPI OpenStorageProvider(
	_Out_   NCRYPT_PROV_HANDLE *phProvider,			///< provider handle
	_In_opt_ LPCWSTR pszProviderName,
	_In_    DWORD dwFlags)
{
	UNREFERENCED_PARAMETER(dwFlags);

	if ((phProvider == nullptr) || (pszProviderName == nullptr))
		return NTE_INVALID_PARAMETER;

	ProviderHandle* handle = new ProviderHandle;
	handle->name = pszProviderName;

	*phProvider = reinterpret_cast<NCRYPT_PROV_HANDLE>(handle);

	return ERROR_SUCCESS;
}

/// Release a handle to the provider
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
SECURITY_STATUS WINAPI FreeProvider(
	_In_    NCRYPT_PROV_HANDLE hProvider)		///< provider handle
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	delete handle;

	return ERROR_SUCCESS;
}
//
//
///******************************************************************************
//* DESCRIPTION :     Open a key in the SAMPLE key storage provider
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider    A handle to the sample KSP provider
//*            LPCWSTR pszKeyName              Name of the key
//             DWORD  dwLegacyKeySpec          Flags for legacy key support (unused)
//*            DWORD   dwFlags                 Flags (unused)
//* OUTPUTS:
//*            NCRYPT_KEY_HANDLE               A handle to the opened key
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*                                            provider handle.
//*            NTE_INVALID_PARAMETER           One or more parameters are invalid.
//*            NTE_NO_MEMORY                   A memory allocation failure occurred.
//*/

// Open a key in the phone
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
_Check_return_ KSP_STATUS WINAPI OpenKey(
	_In_    NCRYPT_PROV_HANDLE hProvider,	///< provider handle
	_Out_   NCRYPT_KEY_HANDLE *phKey,		///< key handle
	_In_    LPCWSTR pszKeyName,				///< key name
	_In_opt_ DWORD  dwLegacyKeySpec,		///< legacy spec (not used)
	_In_    DWORD   dwFlags)				///< flags (not used)
{
	UNREFERENCED_PARAMETER(dwLegacyKeySpec);
	UNREFERENCED_PARAMETER(dwFlags);

	try
	{
		auto ValidateProviser = [&](bool) { return ValidateProviderHandle2(hProvider); };

		auto FindKey = [&](ProviderHandle* provider) 
		{
			return provider->FindKey2(pszKeyName); 
		};

		auto ReturnKeyHandle = [&](std::unique_ptr<KeyHandle> key) 
		{
			*phKey = reinterpret_cast<NCRYPT_KEY_HANDLE>(key.release());
		};


		//auto k = ValidatePointers(phKey, pszKeyName)
		//	.map(ValidateProviser)
		//	//.and_then(ValidateProviser)
		//	.map(FindKey)
		//	//.map(ReturnKeyHandle);

		////auto handle = ValidateProviderHandle2(hProvider);
		////if (!handle) return handle.error();
		////auto x = handle
		////	.and_then(FindKey)
		////	.map(ReturnKeyHandle)
		////	;

		//if (k) 
			//return ERROR_SUCCESS;
		//else 
		//	return k.error();

		//(*handle)->FindKey2(pszKeyName)
		//	.and_then(FindKey)
		//	.map(ReturnKeyHandle);

		//auto key = (*handle)->FindKey2(pszKeyName)
		//	.map([](std::unique_ptr<KeyHandle>& k) { return k.release(); })
		//	.map([&](auto& k) { *phKey = reinterpret_cast<NCRYPT_KEY_HANDLE>(k); return ERROR_SUCCESS; });

		//return key ? *key : key.error();
			/*.and_then([]() {}*/;

		//return 

		//	.and_then([](auto k) { return k.release(); });


		//	.and_then(ValidateProviderHandle2)
		//auto handle = ValidateProviderHandle(hProvider);

		//auto p = ValidatePointers(phKey, pszKeyName);
		//if (!p) return p.error();

		//auto keyHandle = handle->FindKey(pszKeyName);

		//*phKey = reinterpret_cast<NCRYPT_KEY_HANDLE>(keyHandle.release());

		return ERROR_SUCCESS;
	}
	catch (...)
	{
		return NTE_INVALID_HANDLE;
	}
}

/// Create a new key
/// \return NTE_NOT_SUPPORTED
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
_Check_return_ KSP_STATUS WINAPI CreatePersistedKey(
	_In_    NCRYPT_PROV_HANDLE hProvider,	///< provider handle
	_Out_   NCRYPT_KEY_HANDLE *phKey,		///< new key
	_In_    LPCWSTR pszAlgId,				///< algorithm id
	_In_opt_ LPCWSTR pszKeyName,			///< key name
	_In_    DWORD   dwLegacyKeySpec,		///< legacy spec (not used)
	_In_    DWORD   dwFlags)				///< flags (not used)
{
	UNREFERENCED_PARAMETER(pszKeyName);
	UNREFERENCED_PARAMETER(dwLegacyKeySpec);

	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	if ((phKey == nullptr) || (pszAlgId == nullptr))
		return  NTE_INVALID_PARAMETER;

	if ((dwFlags & ~(NCRYPT_SILENT_FLAG | NCRYPT_OVERWRITE_KEY_FLAG)) != 0)
	{
		return NTE_BAD_FLAGS;
	}

	if (wcscmp(pszAlgId, NCRYPT_RSA_ALGORITHM) != 0)
	{
		return NTE_NOT_SUPPORTED;
	}

	return NTE_NOT_SUPPORTED;
}


/// Retrieves the value of a named property on the provider
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
/// \retval NTE_NOT_SUPPORTED The property is not supported.
/// \retval NTE_BUFFER_TOO_SMALL Output buffer is too small.
_Check_return_ KSP_STATUS WINAPI GetProviderProperty(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    LPCWSTR pszProperty,					///< Name of the property
	_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput,		///< Output buffer containing the value of the property.  If pbOutput is NULL, required buffer size will return in *pcbResult.
	_In_    DWORD   cbOutput,						///< Size of the output buffer
	_Out_   DWORD * pcbResult,						///< Required size of the output buffer
	_In_    DWORD   dwFlags)						///< flags
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	if ((pszProperty == nullptr) || (pcbResult == nullptr))
		return NTE_INVALID_PARAMETER;

	if (wcslen(pszProperty) > NCRYPT_MAX_PROPERTY_NAME)
		return NTE_INVALID_PARAMETER;

	if ((dwFlags & ~(NCRYPT_SILENT_FLAG)) != 0)
		return NTE_BAD_FLAGS;

	DWORD property = 0;
	DWORD cbResult = 0;

	if (wcscmp(pszProperty, NCRYPT_IMPL_TYPE_PROPERTY) == 0)
	{
		property = KSP_IMPL_TYPE_PROPERTY;
		cbResult = sizeof(DWORD);
	}
	else if (wcscmp(pszProperty, NCRYPT_MAX_NAME_LENGTH_PROPERTY) == 0)
	{
		property = KSP_MAX_NAME_LENGTH_PROPERTY;
		cbResult = sizeof(DWORD);
	}
	else if (wcscmp(pszProperty, NCRYPT_NAME_PROPERTY) == 0)
	{
		property = KSP_NAME_PROPERTY;
		cbResult = static_cast<DWORD>((handle->name.size() + 1) * sizeof(WCHAR));
	}
	else if (wcscmp(pszProperty, NCRYPT_VERSION_PROPERTY) == 0)
	{
		property = KSP_VERSION_PROPERTY;
		cbResult = sizeof(DWORD);
	}
	else if (wcscmp(pszProperty, NCRYPT_USE_CONTEXT_PROPERTY) == 0)
	{
		property = KSP_USE_CONTEXT_PROPERTY;
		cbResult = static_cast<DWORD>((handle->context.size() + 1) * sizeof(WCHAR));
	}
	//    else if(wcscmp(pszProperty, NCRYPT_SECURITY_DESCR_SUPPORT_PROPERTY) == 0)
	//    {
	//        dwProperty = SAMPLEKSP_SECURITY_DESCR_SUPPORT_PROPERTY;
	//        cbResult = sizeof(DWORD);
	//    }
	else
	{
		return NTE_NOT_SUPPORTED;
	}

	*pcbResult = cbResult;
	*pcbResult = cbResult;

	if (pbOutput == NULL)
	{
		return ERROR_SUCCESS;
	}

	if (cbOutput < *pcbResult)
	{
		return NTE_BUFFER_TOO_SMALL;
	}

	switch (property)
	{
	case KSP_IMPL_TYPE_PROPERTY:
		*(DWORD *)pbOutput = NCRYPT_IMPL_SOFTWARE_FLAG | NCRYPT_IMPL_REMOVABLE_FLAG;
		break;

	case KSP_MAX_NAME_LENGTH_PROPERTY:
		*(DWORD *)pbOutput = MAX_PATH;
		break;

	case KSP_NAME_PROPERTY:
		memcpy(pbOutput, handle->name.c_str(), cbResult);
		break;

	case KSP_VERSION_PROPERTY:
		*(DWORD *)pbOutput = KSP_VERSION;
		break;

	case KSP_USE_CONTEXT_PROPERTY:
		memcpy(pbOutput, handle->context.c_str(), cbResult);
		break;

		//        case SAMPLEKSP_SECURITY_DESCR_SUPPORT_PROPERTY:
		//            *(DWORD *)pbOutput = SAMPLEKSP_SUPPORT_SECURITY_DESCRIPTOR ;
		//            break;
	}

	return ERROR_SUCCESS;
}

/// Retrieves the value of a named property for a key object.
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_NOT_SUPPORTED The property is not supported.
/// \retval NTE_BUFFER_TOO_SMALL Output buffer is too small.
_Check_return_ KSP_STATUS WINAPI GetKeyProperty(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,					///< key handle
	_In_    LPCWSTR pszProperty,					///< Name of the property
	_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput,		///< Output buffer containing the value of the property.  If pbOutput is NULL, required buffer size will return in *pcbResult.
	_In_    DWORD   cbOutput,						///< Size of the output buffer
	_Out_   DWORD * pcbResult,						///< Required size of the output buffer
	_In_    DWORD   dwFlags)						///< flags
{
	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	if ((pszProperty == nullptr) ||
		(wcslen(pszProperty) > NCRYPT_MAX_PROPERTY_NAME) ||
		(pcbResult == nullptr))
	{
		return NTE_INVALID_PARAMETER;
	}

	dwFlags &= ~NCRYPT_SILENT_FLAG;

	if (dwFlags & ~NCRYPT_PERSIST_ONLY_FLAG)
	{
		return NTE_BAD_FLAGS;
	}

	if (dwFlags & NCRYPT_PERSIST_ONLY_FLAG)
	{
		return NTE_NOT_SUPPORTED;
	}

	DWORD property = 0;
	DWORD cbResult = 0;

	if (wcscmp(pszProperty, NCRYPT_ALGORITHM_PROPERTY) == 0)
	{
		property = KSP_ALGORITHM_PROPERTY;
		cbResult = (DWORD)(wcslen(BCRYPT_RSA_ALGORITHM) + 1) * sizeof(WCHAR);
	}
	//    else if(wcscmp(pszProperty, NCRYPT_BLOCK_LENGTH_PROPERTY) == 0)
	//    {
	//        dwProperty = SAMPLEKSP_BLOCK_LENGTH_PROPERTY;
	//        cbResult = sizeof(DWORD);
	//    }
	//    else if(wcscmp(pszProperty, NCRYPT_EXPORT_POLICY_PROPERTY) == 0)
	//    {
	//        dwProperty = SAMPLEKSP_EXPORT_POLICY_PROPERTY;
	//        cbResult = sizeof(DWORD);
	//    }
	//    else if(wcscmp(pszProperty, NCRYPT_KEY_USAGE_PROPERTY) == 0)
	//    {
	//        dwProperty = SAMPLEKSP_KEY_USAGE_PROPERTY;
	//        cbResult = sizeof(DWORD);
	//    }
	else if (wcscmp(pszProperty, NCRYPT_KEY_TYPE_PROPERTY) == 0)
	{
		property = KSP_KEY_TYPE_PROPERTY;
		cbResult = sizeof(DWORD);
	}
	//    else if(wcscmp(pszProperty, NCRYPT_LENGTH_PROPERTY) == 0)
	//    {
	//        dwProperty = SAMPLEKSP_LENGTH_PROPERTY;
	//        cbResult = sizeof(DWORD);
	//    }
	//    else if(wcscmp(pszProperty, NCRYPT_LENGTHS_PROPERTY) == 0)
	//    {
	//        dwProperty = SAMPLEKSP_LENGTHS_PROPERTY;
	//        cbResult = sizeof(NCRYPT_SUPPORTED_LENGTHS);
	//    }
	else if (wcscmp(pszProperty, NCRYPT_NAME_PROPERTY) == 0)
	{
		property = KSP_NAME_PROPERTY;
		cbResult = static_cast<DWORD>((key->key->Name.length() + 1) * sizeof(WCHAR));
	}
	//    else if(wcscmp(pszProperty, NCRYPT_SECURITY_DESCR_PROPERTY) == 0)
	//    {
	//         //@@Synchronization schemes would need to be added here for
	//		 //multi-threaded support@@.
	//         dwProperty = SAMPLEKSP_SECURITY_DESCR_PROPERTY;
	//         Status = GetSecurityOnKeyFile(
	//                        pKey,
	//                        dwFlags,
	//                        (PSECURITY_DESCRIPTOR*)&pbSecurityInfo,
	//                        &cbSecurityInfo);
	//         if(Status != ERROR_SUCCESS)
	//         {
	//            goto cleanup;
	//         }
	//
	//         cbResult = cbSecurityInfo;
	//    }
	else if (wcscmp(pszProperty, NCRYPT_ALGORITHM_GROUP_PROPERTY) == 0)
	{
		property = KSP_ALGORITHM_GROUP_PROPERTY;
		cbResult = (DWORD)(wcslen(NCRYPT_RSA_ALGORITHM_GROUP) + 1) * sizeof(WCHAR);
	}
	else if (wcscmp(pszProperty, NCRYPT_UNIQUE_NAME_PROPERTY) == 0)
	{
		property = KSP_UNIQUE_NAME_PROPERTY;
		cbResult = static_cast<DWORD>((key->key->Name.length() + 1) * sizeof(WCHAR));
	}
	//else if (wcscmp(pszProperty, NCRYPT_USE_CONTEXT_PROPERTY) == 0)
	//{
	//	property = KSP_USE_CONTEXT_PROPERTY;
	//	cbResult = static_cast<DWORD>((key->context.size() + 1) * sizeof(WCHAR));
	//}
	else if (wcscmp(pszProperty, NCRYPT_CERTIFICATE_PROPERTY) == 0)
	{
		property = KSP_CERTIFICATE_PROPERTY;
		cbResult = (DWORD)key->key->NumCertificateBytes; //the size of the certificate that is associated with the key
	}
	else
	{
		return NTE_NOT_SUPPORTED;
	}

	*pcbResult = cbResult;

	if (pbOutput == NULL)
	{
		return ERROR_SUCCESS;
	}

	if (cbOutput < *pcbResult)
	{
		return NTE_BUFFER_TOO_SMALL;
	}

	switch (property)
	{
	case KSP_ALGORITHM_PROPERTY:
		memcpy(pbOutput, BCRYPT_RSA_ALGORITHM, cbResult);
		break;
		//
		//    case SAMPLEKSP_BLOCK_LENGTH_PROPERTY:
		//        *(DWORD *)pbOutput =(pKey->dwKeyBitLength+7)/8;
		//        break;
		//
		//    case SAMPLEKSP_EXPORT_POLICY_PROPERTY:
		//        *(DWORD *)pbOutput = pKey->dwExportPolicy;
		//        break;
		//
		//    case SAMPLEKSP_KEY_USAGE_PROPERTY:
		//        *(DWORD *)pbOutput = pKey->dwKeyUsagePolicy;
		//        break;
		//
	case KSP_KEY_TYPE_PROPERTY:
		*(DWORD *)pbOutput = 0;
		break;
		//
		//    case SAMPLEKSP_LENGTH_PROPERTY:
		//        *(DWORD *)pbOutput = pKey->dwKeyBitLength;
		//        break;
		//
		//    case SAMPLEKSP_LENGTHS_PROPERTY:
		//    {
		//        NCRYPT_SUPPORTED_LENGTHS UNALIGNED *pLengths = (NCRYPT_SUPPORTED_LENGTHS *)pbOutput;
		//
		//        Status = BCryptGetProperty(pKey->hProvider,
		//                                   BCRYPT_KEY_LENGTHS,
		//                                   pbOutput,
		//                                   cbOutput,
		//                                   &cbTmp,
		//                                   0);
		//        if (ERROR_SUCCESS != Status)
		//        {
		//            goto cleanup;
		//        }
		//
		//        pLengths->dwDefaultLength = SAMPLEKSP_DEFAULT_KEY_LENGTH;
		//        break;
		//    }
		//
	case KSP_NAME_PROPERTY:
	case KSP_UNIQUE_NAME_PROPERTY:
		memcpy(pbOutput, (const wchar_t*)key->key->Name, cbResult);
		break;

	//case KSP_USE_CONTEXT_PROPERTY:
	//	memcpy(pbOutput, key->context.c_str(), cbResult);
	//	break;

		//    case SAMPLEKSP_SECURITY_DESCR_PROPERTY:
		//        CopyMemory(pbOutput, pbSecurityInfo, cbResult);
		//        break;
		//
	case KSP_ALGORITHM_GROUP_PROPERTY:
		memcpy(pbOutput, NCRYPT_RSA_ALGORITHM_GROUP, cbResult);
		break;

	case KSP_CERTIFICATE_PROPERTY:
	{
		auto psa = key->key->Certificate;
		BYTE* buffer;

		SafeArrayAccessData(psa, (void**)&buffer);
		memcpy(pbOutput, buffer, cbResult);
		SafeArrayUnaccessData(psa);
		break;
	}
	}

	return ERROR_SUCCESS;
}

//
///******************************************************************************
//* DESCRIPTION :  Sets the value for a named property for a CNG key storage
//*                provider object.
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider    A handle to the sample KSP provider
//*            LPCWSTR pszProperty             Name of the property
//*            PBYTE   pbInput                 Input buffer containing the value
//*                                            of the property
//*            DWORD   cbOutput                Size of the input buffer
//*            DWORD   dwFlags                 Flags
//*
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*                                            provider handle.
//*            NTE_INVALID_PARAMETER           One or more parameters are invalid.
//*            NTE_NOT_SUPPORTED               The property is not supported.
//*            NTE_BAD_FLAGS                   dwFlags contains invalid value.
//*            NTE_NO_MEMORY                   A memory allocation failure occurred.
//*/

/// Sets the value for a named property on the provider
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
/// \retval NTE_NOT_SUPPORTED The property is not supported.
_Check_return_ KSP_STATUS WINAPI SetProviderProperty(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    LPCWSTR pszProperty,					///< Name of the property
	_In_reads_bytes_(cbInput) PBYTE pbInput,		///< Input buffer containing the value of the property
	_In_    DWORD   cbInput,						///< Size of the input buffer
	_In_    DWORD   dwFlags)						///< flags
{
	UNREFERENCED_PARAMETER(cbInput);

	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	if ((pszProperty == nullptr) ||
		(wcslen(pszProperty) > NCRYPT_MAX_PROPERTY_NAME) ||
		(pbInput == nullptr))
	{
		return  NTE_INVALID_PARAMETER;
	}

	if ((dwFlags & ~(NCRYPT_SILENT_FLAG)) != 0)
		return  NTE_BAD_FLAGS;

	if (wcscmp(pszProperty, NCRYPT_USE_CONTEXT_PROPERTY) == 0)
	{
		handle->context = reinterpret_cast<LPCWCHAR>(pbInput);
	}
	else
	{
		return NTE_NOT_SUPPORTED;
	}

	return ERROR_SUCCESS;
}

/// Set the value of a named property for a key object.
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_NOT_SUPPORTED The property is not supported.
_Check_return_ KSP_STATUS WINAPI SetKeyProperty(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,					///< key handle
	_In_    LPCWSTR pszProperty,					///< Name of the property
	_In_reads_bytes_(cbInput) PBYTE pbInput,		///< Input buffer containing the value of the property
	_In_    DWORD   cbInput,						///< Size of the input buffer
	_In_    DWORD   dwFlags)						//< flags
{
	UNREFERENCED_PARAMETER(cbInput);

	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	if ((pszProperty == nullptr) ||
		(wcslen(pszProperty) > NCRYPT_MAX_PROPERTY_NAME) ||
		(pbInput == nullptr))
	{
		return NTE_INVALID_PARAMETER;
	}

	dwFlags &= ~NCRYPT_SILENT_FLAG;

	if (wcscmp(pszProperty, NCRYPT_USE_CONTEXT_PROPERTY) == 0)
	{
		key->ChangeContext(reinterpret_cast<LPCWCHAR>(pbInput));
	}
	else
	{
		return NTE_NOT_SUPPORTED;
	}

	return ERROR_SUCCESS;
}
//
///******************************************************************************
//* DESCRIPTION :     Completes a sample key storage key. The key cannot be used
//*                   until this function has been called.
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider    A handle to the sample KSP provider
//*            NCRYPT_KEY_HANDLE hKey          A handle to a sample KSP key
//*            DWORD   dwFlags                 Flags
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*                                            provider handle.
//*            NTE_INVALID_PARAMETER           One or more parameters are invalid.
//*            NTE_NO_MEMORY                   A memory allocation failure occurred.
//*            NTE_BAD_FLAGS                   The dwFlags parameter contains a
//*                                            value that is not valid.
//*/

/// Completes a key. The key cannot be used until this function has been called.
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
_Check_return_ KSP_STATUS WINAPI FinalizeKey(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,					///< key handle
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	SECURITY_STATUS rtn = NTE_INTERNAL_ERROR;
	UNREFERENCED_PARAMETER(dwFlags);
	//    SECURITY_STATUS Status = NTE_INTERNAL_ERROR;
//
//    if (pKey->fFinished == TRUE)
//    {
//        Status = NTE_INVALID_HANDLE;
//        goto cleanup;
//    }
//
//    if((dwFlags & ~(NCRYPT_NO_KEY_VALIDATION |
//                    NCRYPT_WRITE_KEY_TO_LEGACY_STORE_FLAG |
//                    NCRYPT_SILENT_FLAG )) != 0)
//    {
//        Status = NTE_BAD_FLAGS;
//        goto cleanup;
//    }
//
//    if (dwFlags & NCRYPT_WRITE_KEY_TO_LEGACY_STORE_FLAG)
//    {
//        Status = NTE_NOT_SUPPORTED;
//        goto cleanup;
//    }
//
//    if (pKey->pbPrivateKey)
//    {
//        //Private key is provisioned by NCryptSetProperty
//        //or NCryptImportKey.
//        //Once the key blob is imported as a BCrypt key handle,
//        //the key is finalized.
//        Status = ImportRsaKeyPair(pKey);
//        if (Status != ERROR_SUCCESS)
//        {
//            goto cleanup;
//        }
//    }
//    else
//    {
//        //
//        //Finalize the key.
//        //
//        Status = FinalizeKey(pKey);
//        if(Status != ERROR_SUCCESS)
//        {
//             goto cleanup;
//        }
//    }
//
//    //
//    //Write key to the file system, if the key is persisted.
//    //
//    //
//    if(pKey->pszKeyName != NULL)
//    {
//        Status = WriteKeyToStore(pKey);
//        if (Status != ERROR_SUCCESS)
//        {
//            goto cleanup;
//        }
//    }
//
//    pKey->fFinished = TRUE;
//    Status = ERROR_SUCCESS;
//
//cleanup:
	return rtn;
}

/// Deletes a key
/// \return NTE_NOT_SUPPORTED
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
SECURITY_STATUS WINAPI DeleteKey(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,					///< key handle
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	if ((dwFlags & ~(NCRYPT_SILENT_FLAG)) != 0)
		return NTE_BAD_FLAGS;

	return NTE_NOT_SUPPORTED;
}
//
///******************************************************************************
//* DESCRIPTION :     Free a CNG sample KSP key object
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider    A handle to the sample KSP provider
//*            NCRYPT_KEY_HANDLE hKey          A handle to a sample KSP key
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*/

/// Free a key object
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
SECURITY_STATUS WINAPI FreeKey(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey)					///< key handle
{
	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	delete key;

	return ERROR_SUCCESS;
}

/// Free a memory buffer object
/// \retval ERROR_SUCCESS OK
SECURITY_STATUS WINAPI FreeBuffer(
	_Pre_notnull_ PVOID   pvInput)		///< The buffer to free.
{
	UNREFERENCED_PARAMETER(pvInput);

	delete pvInput;
	//free(pvInput);

	return ERROR_SUCCESS;
}

/// Encrypts a block of data.
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
_Check_return_ KSP_STATUS WINAPI Encrypt(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,					///< key handle
	_In_reads_bytes_opt_(cbInput) PBYTE pbInput,	///< Plain data to be encrypted.
	_In_    DWORD   cbInput,						///< Size of the plain data.
	_In_opt_ VOID *pPaddingInfo,					///< Padding information if padding sheme is used.
	_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput,		///< Output buffer containing encrypted data.  If pbOutput is NULL, required buffer size will return in *pcbResult.
	_In_    DWORD   cbOutput,						///< Size of the output buffer.
	_Out_   DWORD * pcbResult,						///< Required size of the output buffer
	_In_    DWORD   dwFlags)						///< flags
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	if ((pbInput == nullptr) ||
		(cbInput == 0) ||
		(pcbResult == nullptr))
	{
		return NTE_INVALID_PARAMETER;
	}

	if ((dwFlags & ~(NCRYPT_NO_PADDING_FLAG |
		NCRYPT_PAD_PKCS1_FLAG |
		NCRYPT_PAD_OAEP_FLAG |
		NCRYPT_SILENT_FLAG)) != 0)
	{
		return NTE_BAD_FLAGS;
	}

	UNREFERENCED_PARAMETER(cbOutput);
	UNREFERENCED_PARAMETER(pbOutput);
	UNREFERENCED_PARAMETER(pPaddingInfo);
	//NTSTATUS status = BCryptEncrypt(key->publicKey, pbInput, cbInput, pPaddingInfo, nullptr, 0, pbOutput, cbOutput, pcbResult, dwFlags);
	//return AdjustNtStatus(status);

	return NTE_INTERNAL_ERROR;
}
//
///******************************************************************************
//* DESCRIPTION :  Decrypts a block of data.
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider    A handle to a sample KSP provider
//*                                            object.
//*            NCRYPT_KEY_HANDLE hKey          A handle to a sample KSP key object.
//*            PBYTE   pbInput                 Encrypted data blob.
//*            DWORD   cbInput                 Size of the encrypted data blob.
//*            VOID    *pPaddingInfo           Padding information if padding sheme
//*                                            is used.
//*            DWORD   cbOutput                Size of the output buffer.
//*            DWORD   dwFlags                 Flags
//* OUTPUTS:
//*            PBYTE   pbOutput                Output buffer containing decrypted
//*                                            data.  If pbOutput is NULL,
//*                                            required buffer size will return in
//*                                            *pcbResult.
//*            DWORD * pcbResult               Required size of the output buffer
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_BAD_KEY_STATE               The key identified by the hKey
//*                                            parameter has not been finalized
//*                                            or is incomplete.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*                                            provider or key handle.
//*            NTE_INVALID_PARAMETER           One or more parameters are invalid.
//*            NTE_BUFFER_TOO_SMALL            Output buffer is too small.
//*            NTE_BAD_FLAGS                   dwFlags contains invalid value.
//*/

/// Decrypts a block of data.
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
_Check_return_ KSP_STATUS WINAPI Decrypt(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,					///< key handle
	_In_reads_bytes_opt_(cbInput) PBYTE pbInput,
	_In_    DWORD   cbInput,
	_In_opt_    VOID *pPaddingInfo,
	_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput,
	_In_    DWORD   cbOutput,
	_Out_   DWORD * pcbResult,
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	if ((pbInput == nullptr) ||
		(cbInput == 0) ||
		(pcbResult == nullptr))
	{
		return NTE_INVALID_PARAMETER;
	}

	if ((dwFlags & ~(NCRYPT_NO_PADDING_FLAG |
		NCRYPT_PAD_PKCS1_FLAG |
		NCRYPT_PAD_OAEP_FLAG |
		NCRYPT_SILENT_FLAG)) != 0)
	{
		return NTE_BAD_FLAGS;
	}

	*pcbResult = 0;

	UNREFERENCED_PARAMETER(pPaddingInfo);
	UNREFERENCED_PARAMETER(pbOutput);
	UNREFERENCED_PARAMETER(cbOutput);
	//    SAMPLEKSP_KEY *pKey;
//    DWORD BlockLength = 0;
//    SECURITY_STATUS Status = NTE_INTERNAL_ERROR;
//    NTSTATUS    ntStatus = ERROR_SUCCESS;
//
//    //
//    // Verify that this key is allowed to decrypt.
//    //
//
//    if((pKey->dwKeyUsagePolicy & NCRYPT_ALLOW_DECRYPT_FLAG) == 0)
//    {
//        Status = (DWORD)NTE_PERM;
//        goto cleanup;
//    }
//
//    BlockLength = (pKey->dwKeyBitLength+7)/8;
//
//    if(cbInput != BlockLength)
//    {
//        Status = NTE_INVALID_PARAMETER;
//        goto cleanup;
//    }
//
//    // Decrypt input buffer.
//    ntStatus = BCryptDecrypt(pKey->hPrivateKey,
//                           pbInput,
//                           cbInput,
//                           pPaddingInfo,
//                           NULL,
//                           0,
//                           pbOutput,
//                           cbOutput,
//                           pcbResult,
//                           dwFlags);
//    if(!NT_SUCCESS(ntStatus))
//    {
//        Status=NormalizeNteStatus(ntStatus);
//        goto cleanup;
//    }
//    Status = ERROR_SUCCESS;
//
//cleanup:
	return ERROR_SUCCESS;
}

/// Determines if it supports a specific cryptographic algorithm.
/// \note Only the RSA algorithm is supported
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
/// \retval NTE_NOT_SUPPORTED This algorithm is not supported.
_Check_return_ KSP_STATUS WINAPI IsAlgSupported(
	_In_    NCRYPT_PROV_HANDLE hProvider,		///< provider handle
	_In_    LPCWSTR pszAlgId,
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	if (pszAlgId == nullptr)
		return NTE_INVALID_PARAMETER;

	if ((dwFlags & ~(NCRYPT_SILENT_FLAG)) != 0)
		return NTE_BAD_FLAGS;

	if (wcscmp(pszAlgId, NCRYPT_RSA_ALGORITHM) != 0)
	{
		return NTE_NOT_SUPPORTED;
	}

	return ERROR_SUCCESS;
}
//
///******************************************************************************
//* DESCRIPTION :  Obtains the names of the algorithms that are supported by
//*                the sample key storage provider.
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider    A handle to a sample KSP provider
//*                                            object.
//*            DWORD   dwAlgOperations         The crypto operations that are to
//*                                            be enumerated.
//*            DWORD   dwFlags                 Flags
//*
//* OUTPUTS:
//*            DWORD * pdwAlgCount             Number of supported algorithms.
//*            NCryptAlgorithmName **ppAlgList List of supported algorithms.
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*                                            provider handle.
//*            NTE_INVALID_PARAMETER           One or more parameters are invalid.
//*            NTE_BAD_FLAGS                   dwFlags contains invalid value.
//*            NTE_NOT_SUPPORTED               The crypto operations are not supported.
//*/
/// Obtains the names of the algorithms that are supported by the key storage provider.
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
_Check_return_ KSP_STATUS WINAPI EnumAlgorithms(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    DWORD   dwAlgOperations,
	_Out_   DWORD * pdwAlgCount,
	_Outptr_result_buffer_(*pdwAlgCount) NCryptAlgorithmName **ppAlgList,
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	if ((pdwAlgCount == nullptr) || (ppAlgList == nullptr))
	{
		return NTE_INVALID_PARAMETER;
	}

	*pdwAlgCount = 0;
	if ((dwFlags & ~(NCRYPT_SILENT_FLAG)) != 0)
		return NTE_BAD_FLAGS;

	DWORD cbOutput = 0;

	if (dwAlgOperations == 0 ||
		((dwAlgOperations & NCRYPT_ASYMMETRIC_ENCRYPTION_OPERATION) != 0) ||
		((dwAlgOperations & NCRYPT_SIGNATURE_OPERATION)) != 0)
	{
		cbOutput = sizeof(NCryptAlgorithmName) + sizeof(BCRYPT_RSA_ALGORITHM);
	}
	else
	{
		return NTE_NOT_SUPPORTED;
	}

	void* pbOutput = malloc(cbOutput);
	if (pbOutput == nullptr)
		return NTE_NO_MEMORY;

	NCryptAlgorithmName* algName = reinterpret_cast<NCryptAlgorithmName*>(pbOutput);
	algName->dwClass = NCRYPT_ASYMMETRIC_ENCRYPTION_INTERFACE;
	algName->dwAlgOperations = NCRYPT_ASYMMETRIC_ENCRYPTION_OPERATION | NCRYPT_SIGNATURE_OPERATION;
	algName->dwFlags = 0;
	algName->pszName = reinterpret_cast<LPWSTR>(algName + 1);
	memcpy(algName->pszName, BCRYPT_RSA_ALGORITHM, sizeof(BCRYPT_RSA_ALGORITHM));

	*pdwAlgCount = 1;
	*ppAlgList = algName;

	return ERROR_SUCCESS;
}
//
///******************************************************************************
//* DESCRIPTION :  Obtains the names of the keys that are stored by the provider.
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider    A handle to a sample KSP provider
//*                                            object
//*            LPCWSTR pszScope                Unused
//*            NCryptKeyName **ppKeyName       Name of the retrieved key
//*            PVOID * ppEnumState             Enumeration state information
//*            DWORD   dwFlags                 Flags
//*
//* OUTPUTS:
//*            PVOID * ppEnumState             Enumeration state information that
//*                                            is used in subsequent calls to
//*                                            this function.
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*                                            provider handle.
//*            NTE_INVALID_PARAMETER           One or more parameters are invalid.
//*            NTE_BAD_FLAGS                   dwFlags contains invalid value.
//*            NTE_NOT_SUPPORTED               NCRYPT_MACHINE_KEY_FLAG is not
//*                                            supported.
//*            NTE_NO_MEMORY                   A memory allocation failure occurred.
//*/

/// Obtains the names of the keys that are stored on the phone.
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
/// \retval NTE_NOT_SUPPORTED NCRYPT_MACHINE_KEY_FLAG is not supported.
_Check_return_ KSP_STATUS WINAPI EnumKeys(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_opt_ LPCWSTR pszScope,
	_Outptr_ NCryptKeyName **ppKeyName,
	_Inout_ PVOID * ppEnumState,
	_In_    DWORD   dwFlags)
{
	static bool isDone = true;

	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	if ((ppKeyName == nullptr) || (ppEnumState == nullptr))
	{
		return NTE_INVALID_PARAMETER;
	}

	if ((dwFlags & ~(NCRYPT_MACHINE_KEY_FLAG | NCRYPT_SILENT_FLAG)) != 0)
	{
		return NTE_BAD_FLAGS;
	}

	if (dwFlags & NCRYPT_MACHINE_KEY_FLAG)
	{
		return NTE_NOT_SUPPORTED;
	}

	UNREFERENCED_PARAMETER(pszScope);

	if (*ppEnumState == nullptr)
		*ppEnumState = new int(0);

	int& alreadySeen = *(int*)*ppEnumState;

	try
	{
		*ppKeyName = handle->EnumerateKeys(alreadySeen);

		if (*ppKeyName == nullptr)
		{
			*ppKeyName = nullptr;
			return NTE_NO_MORE_ITEMS;
		}
		else
		{
			alreadySeen++;
			return ERROR_SUCCESS;
		}

		//*ppKeyName = new NCryptKeyName{};// (NCryptKeyName*)malloc(sizeof(NCryptKeyName));
		////memset(*ppKeyName, 0, sizeof(NCryptKeyName));

		//SAFEARRAY * psa = handle->ksp->EnumerateKeys2();


		//auto count = SafeArrayGetDim(psa);
		//for (unsigned long i = 0u; i < count; ++i)
		//{
		//	MyidBlueKsp::IBlueKey *kkey;
		//	auto hr = SafeArrayGetElement(psa, (LONG*)&i, &kkey);
		//	UNREFERENCED_PARAMETER(hr);

		//	auto size = kkey->NumCertificateBytes;
		//	printf("%lld", size);

		//	auto b = kkey->Certificate;
		//	UNREFERENCED_PARAMETER(b);

		//	CComSafeArray<BYTE> safeArray;
		//	safeArray.Attach(b);

		//	auto bb = safeArray.GetCount(0);
		//	auto yte = *(BYTE**)safeArray.m_psa;
		//	UNREFERENCED_PARAMETER(yte);

		//	std::vector<unsigned char> buffer;//
		//	buffer.reserve(bb);
		//	for (unsigned long j = 0; j < bb; ++j)
		//	{
		//		auto byite = safeArray.GetAt(j);
		//		buffer.push_back(byite);
		//	}

		//}



		//handle->ksp->EnumerateKeys((long)*ppKeyName);

		//wprintf(L"%s", (*ppKeyName)->pszName);

		//*ppEnumState = new char;// malloc(1);

		////void * voidy = nullptr;
		////auto hr = ::SafeArrayAccessData(keys, &voidy);
		////const auto names = reinterpret_cast<NCryptKeyName*>(voidy);

		////for (auto i = 0u; i < keys->rgsabound[0].cElements; ++i)
		////{
		////	auto x = names[0];
		////}

		////hr = ::SafeArrayUnaccessData(keys);

		//return NTE_OP_OK;
	}
	catch (const _com_error & e)
	{
		auto x = e.ErrorMessage();
		wprintf(L"%s", x);
		return NTE_FAIL;
	}
	catch (...)
	{
		return NTE_FAIL;
	}
	//    SAMPLEKSP_PROVIDER *pProvider = NULL;
//    NCryptKeyName *pKeyName = NULL;
//    SAMPLEKSP_MEMORY_BUFFER *pBuffer = NULL;
//    PVOID pEnumState = NULL;
//    SECURITY_STATUS Status = NTE_INTERNAL_ERROR;
//    UNREFERENCED_PARAMETER(pszScope);
//    // Validate input parameters.
//
//    pEnumState = *ppEnumState;
//    if(pEnumState == NULL)
//    {
//        // Find the first key file.
//        Status = FindFirstKeyFile(
//                        &pEnumState,
//                        &pKeyName);
//
//        if(Status != ERROR_SUCCESS)
//        {
//            goto cleanup;
//        }
//
//        // Allocate structure to hold the returned pEnumState buffer.
//        pBuffer = (SAMPLEKSP_MEMORY_BUFFER*) HeapAlloc(
//                                             GetProcessHeap(),
//                                             0,
//                                             sizeof(SAMPLEKSP_MEMORY_BUFFER));
//        if(pBuffer == NULL)
//        {
//            Status = NTE_NO_MEMORY;
//            goto cleanup;
//        }
//        ZeroMemory(pBuffer, sizeof(SAMPLEKSP_MEMORY_BUFFER));
//
//        // Add the returned pEnumState buffer to a global list, so that
//        // the sample KSP will know the correct way to free the buffer.
//        pBuffer->pvBuffer = pEnumState;
//        //@@Critical section code would need to be added here for multi-threaded support.@@
//        InsertTailList(
//            &g_SampleKspEnumStateList,
//            &pBuffer->List);
//        pBuffer = NULL;
//    }
//    else
//    {
//        // Make sure that the passed in pEnumState buffer is one
//        // that we recognize.
//        if(LookupMemoryBuffer(
//            &g_SampleKspEnumStateList,
//            pEnumState) == NULL)
//        {
//            Status = NTE_INVALID_PARAMETER;
//            goto cleanup;
//        }
//
//        Status = FindNextKeyFile(
//                        pEnumState,
//                        &pKeyName);
//
//        if(Status != ERROR_SUCCESS)
//        {
//            goto cleanup;
//        }
//    }
//
//
//    // Build output data.
//    *ppKeyName = pKeyName;
//    pKeyName = NULL;
//    *ppEnumState = pEnumState;
//    pEnumState = NULL;
//
//    Status = ERROR_SUCCESS;
//cleanup:
//    if(pKeyName)
//    {
//        HeapFree(GetProcessHeap(),0,pKeyName);
//    }
//
//    if(pBuffer)
//    {
//        HeapFree(GetProcessHeap(),0,pBuffer);
//    }
//
//    if (pEnumState)
//    {
//        HeapFree(GetProcessHeap(),0,pEnumState);
//    }
//
}

/// Imports a key from a memory BLOB.
/// \return NTE_NOT_SUPPORTED
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
_Check_return_ KSP_STATUS WINAPI ImportKey(
	_In_    NCRYPT_PROV_HANDLE hProvider,		///< provider handle
	_In_opt_ NCRYPT_KEY_HANDLE hImportKey,
	_In_    LPCWSTR pszBlobType,
	_In_opt_ NCryptBufferDesc *pParameterList,
	_Out_   NCRYPT_KEY_HANDLE *phKey,
	_In_reads_bytes_(cbData) PBYTE pbData,
	_In_    DWORD   cbData,
	_In_    DWORD   dwFlags)
{
	UNREFERENCED_PARAMETER(hImportKey);
	UNREFERENCED_PARAMETER(pParameterList);

	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	if (phKey == nullptr)
	{
		return NTE_INVALID_PARAMETER;
	}

	*phKey = NULL;
	if ((pbData == nullptr) || (cbData == 0) || (pszBlobType == nullptr))
	{
		return NTE_INVALID_PARAMETER;
	}

	if ((dwFlags & ~(NCRYPT_MACHINE_KEY_FLAG | BCRYPT_NO_KEY_VALIDATION |
		NCRYPT_WRITE_KEY_TO_LEGACY_STORE_FLAG | NCRYPT_DO_NOT_FINALIZE_FLAG |
		NCRYPT_SILENT_FLAG | NCRYPT_OVERWRITE_KEY_FLAG)) != 0)
	{
		return NTE_BAD_FLAGS;
	}

	if (dwFlags & (NCRYPT_MACHINE_KEY_FLAG |
		NCRYPT_WRITE_KEY_TO_LEGACY_STORE_FLAG))
	{
		return NTE_NOT_SUPPORTED;
	}

	return NTE_NOT_SUPPORTED;
}
//
///******************************************************************************
//* DESCRIPTION :  Exports a sample key storage key into a memory BLOB.
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider     A handle to a sample KSP provider
//*                                             object.
//*            NCRYPT_KEY_HANDLE hKey           A handle to the sample KSP key
//*                                             object to export.
//*            NCRYPT_KEY_HANDLE hExportKey     Unused
//*            LPCWSTR pszBlobType              Type of the key blob.
//*            NCryptBufferDesc *pParameterList Additional parameter information.
//*            DWORD   cbOutput                 Size of the key blob.
//*            DWORD   dwFlags                  Flags
//*
//* OUTPUTS:
//*            PBYTE pbOutput                  Key blob.
//*            DWORD * pcbResult               Required size of the key blob.
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*                                            provider handle.
//*            NTE_INVALID_PARAMETER           One or more parameters are invalid.
//*            NTE_BAD_FLAGS                   dwFlags contains invalid value.
//*            NTE_NOT_SUPPORTED               The type of the key blob is not
//*                                            supported.
//*            NTE_NO_MEMORY                   A memory allocation failure occurred.
//*            NTE_INTERNAL_ERROR              Encoding failed.
//*/

/// Exports a key into a memory BLOB.
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
/// \retval NTE_NOT_SUPPORTED The type of the key blob is not supported.
_Check_return_ KSP_STATUS WINAPI ExportKey(
	_In_    NCRYPT_PROV_HANDLE hProvider,		///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,				///< key handle
	_In_opt_ NCRYPT_KEY_HANDLE hExportKey,
	_In_    LPCWSTR pszBlobType,
	_In_opt_ NCryptBufferDesc *pParameterList,
	_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput,
	_In_    DWORD   cbOutput,
	_Out_   DWORD * pcbResult,
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	if (pcbResult == nullptr)
	{
		return NTE_INVALID_PARAMETER;
	}

	*pcbResult = NULL;
	if (pszBlobType == nullptr)
	{
		return NTE_INVALID_PARAMETER;
	}

	if ((dwFlags & ~(NCRYPT_SILENT_FLAG | NCRYPT_EXPORT_LEGACY_FLAG)) != 0)
	{
		return NTE_BAD_FLAGS;
	}
	if (dwFlags & NCRYPT_EXPORT_LEGACY_FLAG)
	{
		return NTE_NOT_SUPPORTED;
	}

	if ((wcscmp(pszBlobType, BCRYPT_PRIVATE_KEY_BLOB) == 0) ||
		(wcscmp(pszBlobType, BCRYPT_RSAPRIVATE_BLOB) == 0) ||
		(wcscmp(pszBlobType, BCRYPT_RSAFULLPRIVATE_BLOB) == 0) ||
		(wcscmp(pszBlobType, NCRYPT_PKCS7_ENVELOPE_BLOB) == 0) ||
		(wcscmp(pszBlobType, NCRYPT_PKCS8_PRIVATE_KEY_BLOB) == 0))
	{
		return NTE_NOT_SUPPORTED;
	}

	if ((wcscmp(pszBlobType, BCRYPT_PUBLIC_KEY_BLOB) != 0) &&
		(wcscmp(pszBlobType, BCRYPT_RSAPUBLIC_BLOB) != 0))
	{
		return NTE_INVALID_PARAMETER;
	}

	UNREFERENCED_PARAMETER(hExportKey);
	UNREFERENCED_PARAMETER(pParameterList);
	UNREFERENCED_PARAMETER(pbOutput);
	UNREFERENCED_PARAMETER(cbOutput);
	SECURITY_STATUS rtn = NTE_INTERNAL_ERROR;

	//    //Export the public key blob.
	//    if (fPublicKeyBlob)
	//    {
	//        // Obtain the key blob from the primitive layer.
	//        ntStatus = BCryptExportKey(
	//                        pKey->hPublicKey,
	//                        NULL,
	//                        pszBlobType,
	//                        pbOutput,
	//                        cbOutput,
	//                        pcbResult,
	//                        0);
	//        if(!NT_SUCCESS(ntStatus))
	//        {
	//            Status = NormalizeNteStatus(ntStatus);
	//            goto cleanup;
	//        }
	//    }
	return rtn;
}
//
///******************************************************************************
//* DESCRIPTION :  creates a signature of a hash value.
//*
//* INPUTS :
//*            NCRYPT_PROV_HANDLE hProvider    A handle to a sample KSP provider
//*                                            object
//*            NCRYPT_KEY_HANDLE hKey          A handle to a sample KSP key object
//*            VOID    *pPaddingInfo           Padding information is padding sheme
//*                                            is used
//*            PBYTE  pbHashValue              Hash to sign.
//*            DWORD  cbHashValue              Size of the hash.
//*            DWORD  cbSignature              Size of the signature
//*            DWORD  dwFlags                  Flags
//* OUTPUTS:
//*            PBYTE  pbSignature              Output buffer containing signature.
//*                                            If pbOutput is NULL, required buffer
//*                                            size will return in *pcbResult.
//*            DWORD * pcbResult               Required size of the output buffer.
//* RETURN :
//*            ERROR_SUCCESS                   The function was successful.
//*            NTE_BAD_KEY_STATE               The key identified by the hKey
//*                                            parameter has not been finalized
//*                                            or is incomplete.
//*            NTE_INVALID_HANDLE              The handle is not a valid sample KSP
//*                                            provider or key handle.
//*            NTE_INVALID_PARAMETER           One or more parameters are invalid.
//*            NTE_BUFFER_TOO_SMALL            Output buffer is too small.
//*            NTE_BAD_FLAGS                   dwFlags contains invalid value.
//*/

/// Creates a signature of a hash value.
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
	_Check_return_ KSP_STATUS WINAPI SignHash(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,					///< key handle
	_In_opt_    VOID  *pPaddingInfo,
	_In_reads_bytes_(cbHashValue) PBYTE pbHashValue,
	_In_    DWORD   cbHashValue,
	_Out_writes_bytes_to_opt_(cbSignature, *pcbResult) PBYTE pbSignature,
	_In_    DWORD   cbSignature,
	_Out_   DWORD * pcbResult,
	_In_    DWORD   dwFlags)
{
	try
	{
		auto key = ValidateKeyHandle(hProvider, hKey);
		if (key == nullptr)
			return NTE_INVALID_HANDLE;

		if (pcbResult == nullptr)
			return NTE_INVALID_PARAMETER;

		if (dwFlags & ~(BCRYPT_PAD_PKCS1 | BCRYPT_PAD_PSS | NCRYPT_SILENT_FLAG))
			return NTE_BAD_FLAGS;

		auto array = BufferToByteArray(cbHashValue, pbHashValue);

		if (pbSignature == nullptr)
		{
			*pcbResult = key->CalculateSignatureSize(array, MyidBlueKsp::HashAlgorithm_Pkcs1);
		}
		else
		{
			//auto signature = TakeControl(key->Sign(array));
			//if (!ByteArrayIntoBuffer(signature, cbSignature, pbSignature))
			//	return NTE_BUFFER_TOO_SMALL;
			//*pcbResult = signature.GetCount();
		}

		UNREFERENCED_PARAMETER(pPaddingInfo);
		UNREFERENCED_PARAMETER(pbHashValue);
		UNREFERENCED_PARAMETER(cbHashValue);
		UNREFERENCED_PARAMETER(pbSignature);
		UNREFERENCED_PARAMETER(cbSignature);
		
		return ERROR_SUCCESS;
	}
	catch (const std::exception& e)
	{
		OutputDebugStringA(e.what());
	}
	catch (const _com_error& err)
	{
		OutputDebugString(err.ErrorMessage());
	}
	catch (...)
	{
		OutputDebugString(L"Unknown exception");
	}
		
	return NTE_INTERNAL_ERROR;
}

/// Verifies that the specified signature matches the specified hash
/// \retval ERROR_SUCCESS OK
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
/// \retval NTE_INVALID_PARAMETER One or more parameters are invalid.
/// \retval NTE_BAD_FLAGS dwFlags contains invalid value.
	_Check_return_ KSP_STATUS WINAPI VerifySignature(
	_In_    NCRYPT_PROV_HANDLE hProvider,				///< provider handle
	_In_    NCRYPT_KEY_HANDLE hKey,						///< key handle
	_In_opt_    VOID *pPaddingInfo,						///< Padding information is padding sheme is used.
	_In_reads_bytes_(cbHashValue) PBYTE pbHashValue,	///< Hash data
	_In_    DWORD   cbHashValue,						///< Size of the hash
	_In_reads_bytes_(cbSignature) PBYTE pbSignature,	///< Signature data
	_In_    DWORD   cbSignature,						///< Size of the signature
	_In_    DWORD   dwFlags)							///< flags
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	if ((pbHashValue == nullptr) || (cbHashValue == 0) ||
		(pbSignature == nullptr) || (cbSignature == 0))
	{
		return NTE_INVALID_PARAMETER;
	}

	if (dwFlags & ~(BCRYPT_PAD_PKCS1 | BCRYPT_PAD_PSS | NCRYPT_SILENT_FLAG))
	{
		return NTE_BAD_FLAGS;
	}
	
	UNREFERENCED_PARAMETER(pPaddingInfo);

	//NTSTATUS status = BCryptVerifySignature(key->publicKey, pPaddingInfo, pbHashValue, cbHashValue, pbSignature, cbSignature, dwFlags);
	//return AdjustNtStatus(status);

	return NTE_INTERNAL_ERROR;
}

/// Prompt the user
/// \retval NTE_NOT_SUPPORTED
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
SECURITY_STATUS WINAPI PromptUser(
	_In_    NCRYPT_PROV_HANDLE hProvider,		///< provider handle
	_In_opt_ NCRYPT_KEY_HANDLE hKey,			///< key handle
	_In_    LPCWSTR  pszOperation,
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	UNREFERENCED_PARAMETER(pszOperation);
	UNREFERENCED_PARAMETER(dwFlags);

	return NTE_NOT_SUPPORTED;
}

/// Creates a change key notification event
/// \retval NTE_NOT_SUPPORTED
/// \retval NTE_INVALID_HANDLE the handle is not a valid provider handle
_Check_return_ KSP_STATUS WINAPI NotifyChangeKey(
	_In_    NCRYPT_PROV_HANDLE hProvider,		///< provider handle
	_Inout_ HANDLE *phEvent,
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	UNREFERENCED_PARAMETER(phEvent);
	UNREFERENCED_PARAMETER(dwFlags);

	return NTE_NOT_SUPPORTED;
}

/// Creates a secret agreement value from a private and a public key.
_Check_return_ KSP_STATUS WINAPI SecretAgreement(
	_In_    NCRYPT_PROV_HANDLE hProvider,		///< provider handle
	_In_    NCRYPT_KEY_HANDLE hPrivKey,
	_In_    NCRYPT_KEY_HANDLE hPubKey,
	_Out_   NCRYPT_SECRET_HANDLE *phAgreedSecret,
	_In_    DWORD   dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto privKey = ValidateKeyHandle(hProvider, hPrivKey);
	if (privKey == nullptr)
		return NTE_INVALID_HANDLE;

	auto pubKey = ValidateKeyHandle(hProvider, hPubKey);
	if (pubKey == nullptr)
		return NTE_INVALID_HANDLE;

	UNREFERENCED_PARAMETER(phAgreedSecret);
	UNREFERENCED_PARAMETER(dwFlags);

	return NTE_NOT_SUPPORTED;
}

/// Derives a key.
_Check_return_ KSP_STATUS WINAPI DeriveKey(
	_In_        NCRYPT_PROV_HANDLE   hProvider,			///< provider handle
	_In_        NCRYPT_SECRET_HANDLE hSharedSecret,
	_In_        LPCWSTR              pwszKDF,
	_In_opt_    NCryptBufferDesc     *pParameterList,
	_Out_writes_bytes_to_opt_(cbDerivedKey, *pcbResult) PBYTE pbDerivedKey,
	_In_        DWORD                cbDerivedKey,
	_Out_       DWORD                *pcbResult,
	_In_        ULONG                dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	UNREFERENCED_PARAMETER(hSharedSecret);
	UNREFERENCED_PARAMETER(pwszKDF);
	UNREFERENCED_PARAMETER(pParameterList);
	UNREFERENCED_PARAMETER(pbDerivedKey);
	UNREFERENCED_PARAMETER(cbDerivedKey);
	UNREFERENCED_PARAMETER(pcbResult);
	UNREFERENCED_PARAMETER(dwFlags);

	return NTE_NOT_SUPPORTED;
}

/// Frees a shared secret handle that was created by the provider's shared secret algorithm.
SECURITY_STATUS WINAPI FreeSecret(
	_In_    NCRYPT_PROV_HANDLE hProvider,			///< provider handle
	_In_    NCRYPT_SECRET_HANDLE hSharedSecret)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	UNREFERENCED_PARAMETER(hSharedSecret);

	return NTE_NOT_SUPPORTED;
}

/// Derives a key from a secret or a password.
_Check_return_ KSP_STATUS WINAPI KeyDerivation(
	_In_ NCRYPT_PROV_HANDLE hProvider,				///< provider handle
	_In_ NCRYPT_KEY_HANDLE hKey,					///< key handle
	_In_opt_ NCryptBufferDesc *pParameterList,
	_Out_writes_bytes_to_(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey,
	_In_ DWORD cbDerivedKey,
	_Out_ DWORD *pcbResult,
	_In_ ULONG dwFlags)
{
	ProviderHandle* handle = ValidateProviderHandle(hProvider);
	if (handle == nullptr)
		return NTE_INVALID_HANDLE;

	auto key = ValidateKeyHandle(hProvider, hKey);
	if (key == nullptr)
		return NTE_INVALID_HANDLE;

	UNREFERENCED_PARAMETER(pParameterList);
	UNREFERENCED_PARAMETER(pbDerivedKey);
	UNREFERENCED_PARAMETER(cbDerivedKey);
	UNREFERENCED_PARAMETER(pcbResult);
	UNREFERENCED_PARAMETER(dwFlags);

	return NTE_NOT_SUPPORTED;
}


CComSafeArray<BYTE> BufferToByteArray(unsigned long size, BYTE* buffer)
{
	CComSafeArray<BYTE> array(size);
	
	for (int i = 0; i < (int)size; ++i)
		array[i] = buffer[i];
	
	return array;
}

CComSafeArray<BYTE> TakeControl(SAFEARRAY* array)
{
	CComSafeArray<BYTE> safe;
	safe.Attach(array);
	return safe;
}

bool ByteArrayIntoBuffer(const CComSafeArray<BYTE> & array, unsigned long size, BYTE* buffer)
{
	if (size < array.GetCount()) 
		return false;

	for (int i = 0; i < (int)size; ++i)
		buffer[i] = array[i];

	return true;
}