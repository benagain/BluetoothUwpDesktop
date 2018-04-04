#pragma once

#include "targetver.h"

#include <stdio.h>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <windows.h>
#include <wincrypt.h>
#include <bcrypt.h>
#include <ncrypt.h>
#include "..\CPDK\Include\bcrypt_provider.h"
#include "..\CPDK\Include\ncrypt_provider.h"

