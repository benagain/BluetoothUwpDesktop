#include "stdafx.h"
#include "RegKSP.h"
#include "RegStore.h"


int wmain(int argc, const wchar_t* argv[])
{
	int rtn = 0;
	bool valid = false;

	if (argc > 1)
	{
		if ((_wcsicmp(argv[1], L"-register-ksp") == 0) ||
			(_wcsicmp(argv[1], L"-rk") == 0))
		{
			RegisterKSP();
			valid = true;
		}
		else if ((_wcsicmp(argv[1], L"-unregister-ksp") == 0) ||
			(_wcsicmp(argv[1], L"-uk") == 0))
		{
			UnRegisterKSP();
			valid = true;
		}
		else if ((_wcsicmp(argv[1], L"-register-store") == 0) ||
			(_wcsicmp(argv[1], L"-rs") == 0))
		{
			RegisterStore();
			valid = true;
		}
		else if ((_wcsicmp(argv[1], L"-unregister-store") == 0) ||
			(_wcsicmp(argv[1], L"-us") == 0))
		{
			UnRegisterStore();
			valid = true;
		}
		else
		{
			std::wcout << L"Unrecognised command \"" << argv[1] << "\"" << std::endl;
		}
	}

	if (!valid)
	{
		std::wcout << L"Usage:" << std::endl;
		std::wcout << L"InstallPhoneKSP" << std::endl;
		std::wcout << L"    -register-ksp      (-rk) Register Phone KSP" << std::endl;
		std::wcout << L"    -unregister-ksp    (-uk) Unregister Phone KSP" << std::endl;
		std::wcout << L"    -register-store    (-rs) Register Phone certificate store" << std::endl;
		std::wcout << L"    -unregister-store  (-us) Unregister Phone certificate store" << std::endl;
		rtn = 1;
	}

	return rtn;
}

