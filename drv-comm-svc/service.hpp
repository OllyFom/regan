#pragma once

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "logger/logger.h"

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "kernel32.lib")

#define SVCNAME TEXT("DevCommSvc")
#define SVCDISPLAY TEXT("Cлужба для коммуникации с драйвером DevCommSvc")
#define SVCDESCRIPTION TEXT("Данная служба разрабатыватся для дипломной работы")

#define WITHOUT_CONST(x) const_cast<char*>(x)

VOID SvcInstall(void);
