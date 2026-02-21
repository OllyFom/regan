#include "svc-manager.hpp"

#define SERVICE_NAME "drv-comm-svc"
// TODO: change path after creation install cmake
#define SERVICE_PATH "D:/builds/regan-srv-mgr/drv-comm-svc/Debug/drv-comm-svc.exe"

ServiceMgr::ServiceMgr()
{
    // std::cout << "Service Manager constructor" << std::endl;
    hMgr_ = OpenSCManagerA(
        NULL,
        NULL,
        SC_MANAGER_ALL_ACCESS);
}

// TODO:
bool ServiceMgr::Running()
{
    return true;
}

bool ServiceMgr::Install()
{
    if (hMgr_ == NULL)
    {
        std::cout << "Try to connect to SCM" << std::endl;
        hMgr_ = OpenSCManagerA(
            NULL,
            NULL,
            SC_MANAGER_ALL_ACCESS);
    }
    if (hMgr_ == NULL)
    {
        std::cout << "Failed to connect to SCM" << std::endl;
        return false;
    }

    SetLastError(ERROR_SUCCESS);

    if (hSvc_ != NULL)
    {
        std::string func_name = "OpenServiceA";
        hSvc_ = OpenServiceA(
            hMgr_,
            SERVICE_NAME,
            SC_MANAGER_ALL_ACCESS);
        DWORD error = GetLastError();
        if (!CheckErrorCode(func_name, error))
        {
            CloseServiceHandle(hSvc_);
            return false;
        }
    }
    else
    {
        std::string func_name = "CreateServiceA";
        hSvc_ = CreateServiceA(
            hMgr_,
            SERVICE_NAME,
            SERVICE_NAME,
            SC_MANAGER_ALL_ACCESS,
            SERVICE_WIN32_OWN_PROCESS,
            SERVICE_AUTO_START,
            SERVICE_ERROR_CRITICAL,
            SERVICE_PATH,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL);
        DWORD error = GetLastError();

        if (!CheckErrorCode(func_name, error))
        {
            CloseServiceHandle(hSvc_);
            return false;
        }
    }

    return true;
}
bool ServiceMgr::Uninstall()
{
    // TODO: add ControlService for stop running service before Deleting
    bool uninstalled = DeleteService(hSvc_);
    DWORD error = GetLastError();

    std::string func_name = "DeleteService";
    if (!CheckErrorCode(func_name, error))
    {
        return false;
    }

    CloseServiceHandle(hSvc_);
    CloseServiceHandle(hMgr_);
    return uninstalled;
}

ServiceMgr::~ServiceMgr()
{
    // std::cout << "Service Manager destructor" << std::endl;
    CloseServiceHandle(hSvc_);
    CloseServiceHandle(hMgr_);
}

// TODO: refactoring debug function
bool ServiceMgr::CheckErrorCode(const std::string &func_name, const DWORD error)
{

    if (error == ERROR_INVALID_PARAMETER)
    {
        std::cout << "Failed function: " << func_name << ", with error: " << "ERROR_INVALID_PARAMETER" << std::endl;
        return false;
    }
    if (error == ERROR_INVALID_NAME)
    {
        std::cout << "Failed function: " << func_name << ", with error: " << "ERROR_INVALID_NAME" << std::endl;
        return false;
    }
    if (error == ERROR_INVALID_HANDLE)
    {
        std::cout << "Failed function: " << func_name << ", with error: " << "ERROR_INVALID_HANDLE" << std::endl;
        return false;
    }
    if (error == ERROR_ACCESS_DENIED)
    {
        std::cout << "Failed function: " << func_name << ", with error: " << "ERROR_ACCESS_DENIED" << std::endl;
        return false;
    }
    if (error != ERROR_SUCCESS)
    {
        std::cout << "Failed function: " << func_name << ". with error code: " << error << std::endl;
        return false;
    }
    return true;
}
