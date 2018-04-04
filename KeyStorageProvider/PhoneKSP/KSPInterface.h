#pragma once

typedef _Return_type_success_(return >= 0) SECURITY_STATUS KSP_STATUS;

_Check_return_ KSP_STATUS WINAPI OpenStorageProvider(_Out_ NCRYPT_PROV_HANDLE *phProvider, _In_opt_ LPCWSTR pszProviderName, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI EnumAlgorithms(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ DWORD dwAlgClass, _Out_ DWORD * pdwAlgCount, _Outptr_result_buffer_(*pdwAlgCount) NCryptAlgorithmName **ppAlgList, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI IsAlgSupported(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ LPCWSTR pszAlgId, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI EnumKeys(_In_ NCRYPT_PROV_HANDLE hProvider, _In_opt_ LPCWSTR pszScope, _Outptr_ NCryptKeyName **ppKeyName, _Inout_ PVOID * ppEnumState, _In_ DWORD dwFlags);
SECURITY_STATUS WINAPI FreeBuffer(_Pre_notnull_ PVOID pvInput);
_Check_return_ KSP_STATUS WINAPI OpenKey(_In_ NCRYPT_PROV_HANDLE hProvider, _Out_ NCRYPT_KEY_HANDLE *phKey, _In_ LPCWSTR pszKeyName, _In_opt_ DWORD dwLegacyKeySpec, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI CreatePersistedKey(_In_ NCRYPT_PROV_HANDLE hProvider, _Out_ NCRYPT_KEY_HANDLE *phKey, _In_ LPCWSTR pszAlgId, _In_opt_ LPCWSTR pszKeyName, _In_ DWORD dwLegacyKeySpec, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI GetProviderProperty(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ LPCWSTR pszProperty, _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput, _In_ DWORD cbOutput, _Out_ DWORD * pcbResult, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI GetKeyProperty(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_ LPCWSTR pszProperty, _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput, _In_ DWORD cbOutput, _Out_ DWORD * pcbResult, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI SetProviderProperty(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ LPCWSTR pszProperty, _In_reads_bytes_(cbInput) PBYTE pbInput, _In_ DWORD cbInput, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI SetKeyProperty(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_ LPCWSTR pszProperty, _In_reads_bytes_(cbInput) PBYTE pbInput, _In_ DWORD cbInput, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI FinalizeKey(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI Encrypt(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_reads_bytes_opt_(cbInput) PBYTE pbInput, _In_ DWORD cbInput, _In_opt_ VOID *pPaddingInfo, _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput, _In_ DWORD cbOutput, _Out_ DWORD * pcbResult, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI Decrypt(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_reads_bytes_opt_(cbInput) PBYTE pbInput, _In_ DWORD cbInput, _In_opt_ VOID *pPaddingInfo, _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput, _In_ DWORD cbOutput, _Out_ DWORD * pcbResult, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI ImportKey(_In_ NCRYPT_PROV_HANDLE hProvider, _In_opt_ NCRYPT_KEY_HANDLE hImportKey, _In_ LPCWSTR pszBlobType, _In_opt_ NCryptBufferDesc *pParameterList, _Out_ NCRYPT_KEY_HANDLE *phKey, _In_reads_bytes_(cbData) PBYTE pbData, _In_ DWORD cbData, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI ExportKey(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_opt_ NCRYPT_KEY_HANDLE hExportKey, _In_ LPCWSTR pszBlobType, _In_opt_ NCryptBufferDesc *pParameterList, _Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PBYTE pbOutput, _In_ DWORD cbOutput, _Out_ DWORD * pcbResult, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI SignHash(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_opt_ VOID *pPaddingInfo, _In_reads_bytes_(cbHashValue) PBYTE pbHashValue, _In_ DWORD cbHashValue, _Out_writes_bytes_to_opt_(cbSignature, *pcbResult) PBYTE pbSignature, _In_ DWORD cbSignature, _Out_ DWORD * pcbResult, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI VerifySignature(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_opt_ VOID *pPaddingInfo, _In_reads_bytes_(cbHashValue) PBYTE pbHashValue, _In_ DWORD cbHashValue, _In_reads_bytes_(cbSignature) PBYTE pbSignature, _In_ DWORD cbSignature, _In_ DWORD dwFlags);
SECURITY_STATUS WINAPI DeleteKey(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_ DWORD dwFlags);
SECURITY_STATUS WINAPI FreeProvider(_In_ NCRYPT_PROV_HANDLE hProvider);
SECURITY_STATUS WINAPI FreeKey(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey);
SECURITY_STATUS WINAPI FreeSecret(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_SECRET_HANDLE hSharedSecret);
SECURITY_STATUS WINAPI PromptUser(_In_ NCRYPT_PROV_HANDLE hProvider, _In_opt_ NCRYPT_KEY_HANDLE hKey, _In_ LPCWSTR pszOperation, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI NotifyChangeKey(_In_ NCRYPT_PROV_HANDLE hProvider, _Inout_ HANDLE *phEvent, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI SecretAgreement(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hPrivKey, _In_ NCRYPT_KEY_HANDLE hPubKey, _Out_ NCRYPT_SECRET_HANDLE *phAgreedSecret, _In_ DWORD dwFlags);
_Check_return_ KSP_STATUS WINAPI DeriveKey(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_SECRET_HANDLE hSharedSecret, _In_ LPCWSTR pwszKDF, _In_opt_ NCryptBufferDesc *pParameterList, _Out_writes_bytes_to_opt_(cbDerivedKey, *pcbResult) PBYTE pbDerivedKey, _In_ DWORD cbDerivedKey, _Out_ DWORD *pcbResult, _In_ ULONG dwFlags);
_Check_return_ KSP_STATUS WINAPI KeyDerivation(_In_ NCRYPT_PROV_HANDLE hProvider, _In_ NCRYPT_KEY_HANDLE hKey, _In_opt_ NCryptBufferDesc *pParameterList, _Out_writes_bytes_to_(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey, _In_ DWORD cbDerivedKey, _Out_ DWORD *pcbResult, _In_ ULONG dwFlags);

