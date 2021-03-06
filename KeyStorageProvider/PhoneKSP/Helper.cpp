#include "stdafx.h"
#include "Helper.h"
#include <ntstatus.h>

SECURITY_STATUS AdjustNtStatus(_In_ NTSTATUS status)
{
	switch (status)
	{
	case STATUS_SUCCESS:
		return ERROR_SUCCESS;

	case STATUS_NO_MEMORY:
	case STATUS_INSUFFICIENT_RESOURCES:
		return NTE_NO_MEMORY;

	case STATUS_INVALID_PARAMETER:
		return  NTE_INVALID_PARAMETER;

	case STATUS_INVALID_HANDLE:
		return NTE_INVALID_HANDLE;

	case STATUS_BUFFER_TOO_SMALL:
		return NTE_BUFFER_TOO_SMALL;

	case STATUS_NOT_SUPPORTED:
		return NTE_NOT_SUPPORTED;

	case STATUS_INTERNAL_ERROR:
	case ERROR_INTERNAL_ERROR:
		return NTE_INTERNAL_ERROR;

	case STATUS_INVALID_SIGNATURE:
		return NTE_BAD_SIGNATURE;
	}

	return NTE_INTERNAL_ERROR;
}

