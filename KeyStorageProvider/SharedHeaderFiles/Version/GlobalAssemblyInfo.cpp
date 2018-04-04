/// Common AssemblyInfo.cpp

// ensure version.h is included before this is included

using namespace System::Reflection;

#include "ExpandVersion.h"		// contains the #defines to turn individual file components into format for .rc files
#include "VendorVersion.h"		// controls the 3rd digit of the product version stamp
#include "VendorIssue.h"		// controls the 4th digit of the product version stamp
#include "ProductVersion.h"
#include "BuildVersion.h"
#include "TlbResourceInclude.h"
#include "VisualStudioBuild.h"

[assembly:AssemblyCompanyAttribute("Intercede Ltd.")];
[assembly:AssemblyCopyrightAttribute(COPYRIGHT_STRING)];
[assembly:AssemblyTrademarkAttribute("edefice")];
[assembly:AssemblyCultureAttribute("")];

#ifdef PRODUCTNAME_STRING
[assembly:AssemblyProductAttribute(PRODUCTNAME_STRING)];
#else
[assembly:AssemblyProductAttribute("MyID")];
#endif

#ifdef INTERNALNAME_STRING
[assembly:AssemblyTitleAttribute(INTERNALNAME_STRING)];
#endif

#ifdef FILEDESCRIPTION_STRING
[assembly:AssemblyDescriptionAttribute(FILEDESCRIPTION_STRING)];
#endif

#ifdef _DEBUG
	[assembly:AssemblyConfigurationAttribute("Debug")];
#else
	[assembly:AssemblyConfigurationAttribute("Release")];
#endif

[assembly:AssemblyInformationalVersionAttribute(PRODUCTVERSION_STRING_DOT)];
[assembly:AssemblyFileVersionAttribute(FILEVERSION_STRING_DOT)];
