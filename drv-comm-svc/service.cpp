#include "service.hpp"
#include "driver-session/driver-session.hpp"
#include "registry-run-monitor/registry-run-mon.hpp"

// Точка входа в службу
int __cdecl _tmain(int argc, TCHAR *argv[])
{
    RegistryRunMonitor monitor(L"C:\\monitoring\\run_snapshot.dat");
    
    // Первый запуск: просто сохраняем текущее состояние как "базовое"
    monitor.CheckAndUpdate();
    monitor.SaveSnapshot();

    DriverSession drv_session;

    // Если получили уведомление от DriverSession, то получаем данные о новых записях и кладем их в бд

    return 0;
}

VOID SvcInstall(void) {
    
}