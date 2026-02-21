#pragma once
#include "../commands_base.hpp"

#define START_OPTION "start"
#define ENABLE_SERVICE_OPTION "enable"
#define DISABLE_SERVICE_OPTION "disable"

class MonitorCommand : public BaseCommand
{
public:
    MonitorCommand()
    {
        //std::cout << "MonitorCommand constructor" << std::endl;
    }
    ~MonitorCommand()
    {
        //std::cout << "MonitorCommand destructor" << std::endl;
    }

private:
    void DisableMonitorService();
    void EnableMonitorService();
    void StartMonitor();
    void StopMonitor();
};