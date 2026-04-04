#include "monitor.hpp"

#include "svc-manager.hpp"

std::string MonitorCommand::GetName() const { return "monitor"; }
std::string MonitorCommand::GetDescription() const { return "Monitoring management commands"; }

void MonitorCommand::AddOptions(po::options_description &desc)
{
    desc.add_options()("status", "Get monitoring status")("start", "Start monitoring service")("stop", "Stop monitoring service")("enable-boot", "Enable monitor to bootstart")("disable-boot", "Disable monitor from bootstart")("help,h", "Show help for this command");
}

int MonitorCommand::Execute(const po::variables_map &vm)
{
    if (vm.count("help"))
    {
        ShowHelp();
        return 0;
    }

    if (vm.count("status"))
    {
        return CmdStatus();
    }
    if (vm.count("start"))
    {
        return CmdStart();
    }
    if (vm.count("stop"))
    {
        return CmdStop();
    }
    if (vm.count("enable-boot"))
    {
        return CmdEnableBoot();
    }
    if (vm.count("disable-boot"))
    {
        return CmdDisableBoot();
    }

    std::cerr << "[!] No command specified. Use --help for usage.\n";
    return 1;
}

void MonitorCommand::ShowHelp()
{
    std::cout << "Usage: regan monitor [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --status      Get monitoring status\n";
    std::cout << "  --start       Start monitoring\n";
    std::cout << "  --stop        Stop monitoring\n";
    std::cout << "  --enable-boot Enable monitor to bootstart\n";
    std::cout << "  --disable-boot Disable monitor from bootstart\n";
    std::cout << "  -h, --help    Show this help\n";
}

int MonitorCommand::CmdStatus()
{
    std::cout << "[*] Checking monitoring status...\n";
    // TODO: Вызов RegistryRunMonitor или проверка статуса сервиса/драйвера
    std::cout << "[+] Monitoring is ACTIVE\n";
    return 0;
}

int MonitorCommand::CmdStart()
{
    std::cout << "[*] Starting monitoring...\n";
    // TODO: Запуск сервиса
    std::cout << "[+] Monitoring started\n";
    return 0;
}

int MonitorCommand::CmdStop()
{
    std::cout << "[*] Stopping monitoring...\n";
    // TODO: Остановка сервиса
    std::cout << "[+] Monitoring stopped\n";
    return 0;
}

int MonitorCommand::CmdEnableBoot()
{
    std::cout << "[*] Enabling bootstart...\n";
    // TODO: Изменение типа запуска сервиса на SERVICE_AUTO_START
    std::cout << "[+] Bootstart enabled\n";
    return 0;
}

int MonitorCommand::CmdDisableBoot()
{
    std::cout << "[*] Disabling bootstart...\n";
    // TODO: Изменение типа запуска сервиса на SERVICE_DEMAND_START
    std::cout << "[+] Bootstart disabled\n";
    return 0;
}