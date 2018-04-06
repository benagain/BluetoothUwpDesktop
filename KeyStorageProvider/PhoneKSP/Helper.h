#pragma once

enum Property
{
	KSP_ALGORITHM_GROUP_PROPERTY,
	KSP_ALGORITHM_PROPERTY,
	KSP_IMPL_TYPE_PROPERTY,
	KSP_KEY_TYPE_PROPERTY,
	KSP_MAX_NAME_LENGTH_PROPERTY,
	KSP_NAME_PROPERTY,
	KSP_UNIQUE_NAME_PROPERTY,
	KSP_USE_CONTEXT_PROPERTY,
	KSP_VERSION_PROPERTY,
	KSP_CERTIFICATE_PROPERTY,
};

#define KSP_VERSION 0x00010000

SECURITY_STATUS AdjustNtStatus(_In_ NTSTATUS NtStatus);

//void ValidatePointers();// {}
//
//template<typename T, typename... Args>
//void ValidatePointers(T p, Args... args)
//{
//	if (p == nullptr) return;
//	ValidatePointers(args...);
//}

inline tl::expected<bool, SECURITY_STATUS> ValidatePointers() { return {}; }

template<typename T, typename... Args>
tl::expected<bool, SECURITY_STATUS> ValidatePointers(T p, Args... args)
{
	if (p == nullptr) return tl::make_unexpected(NTE_INVALID_PARAMETER);
	return ValidatePointers(args...);

}