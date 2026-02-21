#pragma once

#include <windows.h>

#include <iostream>

class ServiceMgr final
{
public:
    ServiceMgr();
    ~ServiceMgr();
    bool Install();
    bool Uninstall();
    bool Running();
private:
    // Debug function
    bool CheckErrorCode(const std::string& func_name, const DWORD error);
private:
    SC_HANDLE hMgr_ = NULL;
    SC_HANDLE hSvc_ = NULL;
};