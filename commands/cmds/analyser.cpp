#include "analyser.hpp"

#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>

#include "accessory/prefetch_parser.hpp"
#include "storage.hpp"

std::vector<std::string> GetPrefetchFileNamesByPsName(const std::string &ps_name);
std::string FormatTimestamp(time_t timestamp);
bool IsSuspiciousPath(const std::wstring &path);
std::string WStringToString(const std::wstring &wstr);

std::string AnalyseCommand::GetName() const
{
    return "analyse";
}

std::string AnalyseCommand::GetDescription() const
{
    return "Analyse bootstart execution files";
}

void AnalyseCommand::AddOptions(po::options_description &desc)
{
    desc.add_options()("get-list", "Get new bootstart execution files list")("all", "Get report about all bootstart execution files")("name", po::value<std::string>(), "Get report by process name (e.g. --name \"notepad.exe\")")("pid", po::value<std::vector<int>>()->multitoken(), "Analyse files for processes by PID list")("help,h", "Show help for this command");
}

int AnalyseCommand::Execute(const po::variables_map &vm)
{
    if (vm.count("help"))
    {
        ShowHelp();
        return 0;
    }

    if (vm.count("get-list"))
    {
        return CmdGetList();
    }
    if (vm.count("all"))
    {
        return CmdAll();
    }
    if (vm.count("name"))
    {
        return CmdByName(vm["name"].as<std::string>());
    }
    if (vm.count("pid"))
    {
        return CmdByPid(vm["pid"].as<std::vector<int>>());
    }

    std::cerr << "[!] No command specified. Use --help for usage.\n";
    return 1;
}

void AnalyseCommand::ShowHelp()
{
    std::cout << "Usage: regan analyse [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --get-list          Get new bootstart execution files list\n";
    std::cout << "  --all               Get report about all bootstart execution files\n";
    std::cout << "  --name <process>    Get report by process name\n";
    std::cout << "  --pid <pids...>     Analyse files for processes by PID list\n";
    std::cout << "  -h, --help          Show this help\n";
}

int AnalyseCommand::CmdGetList()
{
    std::cout << "[*] Getting new bootstart execution files...\n";
    // TODO: Вызов RegistryRunMonitor::GetNewEntries()
    std::cout << "[+] List retrieved\n";
    return 0;
}

int AnalyseCommand::CmdAll()
{
    std::cout << "[*] Generating full report...\n";
    // TODO: Генерация полного отчёта по всем записям
    std::cout << "[+] Report generated\n";
    return 0;
}

int AnalyseCommand::CmdByName(const std::string &ps_name)
{
    std::cout << "[*] Analysing process: " << ps_name << "\n";
    // TODO: сделать запись в файл для всех данных анализа
    std::cout << AnalysePsFiles(ps_name) << std::endl;
    std::cout << "[+] Analysis complete\n";
    return 0;
}

int AnalyseCommand::CmdByPid(const std::vector<int> &pids)
{
    std::cout << "[*] Analysing " << pids.size() << " process(es) by PID...\n";
    for (int pid : pids)
    {
        std::cout << "  - PID: " << pid << "\n";
        // TODO: Получение имени процесса по PID и вызов анализа
    }
    std::cout << "[+] Analysis complete\n";
    return 0;
}

AnalyseCommand::AnalyseCommand()
{
    // std::cout << "AnalyseCommand constructor" << std::endl;
}

void AnalyseCommand::GetFileList()
{
    Storage storage;
    // Fill the map
}

std::string AnalyseCommand::AnalysePsFiles(const std::string &ps_name)
{
    std::stringstream report;

    auto prefetch_files_list = GetPrefetchFileNamesByPsName(ps_name);

    if (prefetch_files_list.empty())
    {
        report << "[!] No prefetch files found for process: " << ps_name << "\n";
        return report.str();
    }

    struct ExecutionRecord
    {
        std::string prefetch_file; // Имя файла, в котором найдено
        time_t exec_time;          // Время запуска
        int run_count;             // Счётчик в этом файле
    };

    std::vector<ExecutionRecord> executions; // Все запуски
    std::set<std::string> all_used_files;    // Все уникальные файлы
    std::set<std::string> suspicious_files;  // Подозрительные пути
    int total_run_count = 0;                 // Суммарное количество запусков

    for (const auto &file_path : prefetch_files_list)
    {
        try
        {
            PrefetchInfo pf_info(file_path);

            auto exec_time = pf_info.GetExecTime();
            auto run_count = pf_info.GetRunCount();
            auto wused_files = pf_info.GetUsedFiles();

            executions.push_back({fs::path(file_path).filename().u8string(),
                                  exec_time,
                                  run_count});

            total_run_count += run_count;

            for (const auto &wfile : wused_files)
            {
                std::string narrow_file = WStringToString(wfile);

                all_used_files.insert(narrow_file);

                if (IsSuspiciousPath(wfile))
                {
                    suspicious_files.insert(narrow_file);
                }
            }
        }
        catch (const std::exception &e)
        {
            report << "[!] Failed to parse " << file_path << ": " << e.what() << "\n";
            continue;
        }
    }

    report << "=== PREFETCH ANALYSIS: " << ps_name << " ===\n\n";

    report << "[EXECUTION STATISTICS]\n";
    report << "  Total run count (aggregated): " << total_run_count << "\n";
    report << "  Unique prefetch files found: " << prefetch_files_list.size() << "\n";
    report << "  Individual records:\n";

    std::sort(executions.begin(), executions.end(),
              [](const auto &a, const auto &b)
              { return a.exec_time < b.exec_time; });

    for (const auto &exec : executions)
    {
        report << "Prefetch file path:  " << exec.prefetch_file << "\n";
        report << "      Last run: " << FormatTimestamp(exec.exec_time) << "\n";
        report << "      Run count in file: " << exec.run_count << "\n";
    }
    report << "\n";

    report << "[TIMING ANOMALIES CHECK]\n";

    if (executions.size() >= 2)
    {

        for (size_t i = 1; i < executions.size(); ++i)
        {
            double diff_secs = difftime(executions[i].exec_time, executions[i - 1].exec_time);
            if (diff_secs < 1.0 && diff_secs >= 0)
            {
                report << "  [!] RAPID EXECUTION: " << diff_secs << "s between runs\n";
                report << "      " << FormatTimestamp(executions[i - 1].exec_time) << " -> "
                       << FormatTimestamp(executions[i].exec_time) << "\n";
            }
        }

        int night_runs = 0;
        for (const auto &exec : executions)
        {
            struct tm timeinfo;
            localtime_s(&timeinfo, &exec.exec_time);
            if (timeinfo.tm_hour >= 0 && timeinfo.tm_hour < 5)
            {
                night_runs++;
            }
        }
        if (night_runs > 0)
        {
            report << "  [!] NIGHT EXECUTIONS: " << night_runs << " runs between 00:00-05:00\n";
        }
    }
    if (executions.empty())
    {
        report << "  (no valid execution times)\n";
    }
    report << "\n";

    report << "[SUSPICIOUS FILE ACCESS]\n";
    report << "  Total unique files referenced: " << all_used_files.size() << "\n";
    report << "  Potentially suspicious paths: " << suspicious_files.size() << "\n";

    if (!suspicious_files.empty())
    {
        report << "  Review these paths:\n";
        for (const auto &file : suspicious_files)
        {
            std::string narrow_file(file.begin(), file.end());
            report << "file path: " << narrow_file << "\n";
        }
    }
    report << "\n";

    report << "[THREAT INDICATORS SUMMARY]\n";
    int threat_score = 0;

    if (total_run_count > 100)
    {
        report << "  [!] High execution frequency: " << total_run_count << " runs\n";
        threat_score += 2;
    }
    if (!suspicious_files.empty())
    {
        report << "  [!] Suspicious file paths detected: " << suspicious_files.size() << " items\n";
        threat_score += 3;
    }
    if (executions.size() > 1)
    {
        // Множество prefetch-файлов = запуск с разных путей/дисков
        report << "  [i] Multiple prefetch files: process executed from different locations?\n";
        threat_score += 1;
    }

    if (threat_score == 0)
    {
        report << "  [+] No obvious indicators found (but always verify manually)\n";
    }
    else
    {
        report << "  Threat score: " << threat_score << "/10 (higher = more suspicious)\n";
    }

    return report.str();
}

AnalyseCommand::~AnalyseCommand()
{
    // std::cout << "AnalyseCommand destructor" << std::endl;
}

/// @brief Функция для перевода строки в верхний регистр
std::string ToUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

/// @brief Получить список путей до prefetch файлов процесса по его имени
std::vector<std::string> GetPrefetchFileNamesByPsName(const std::string &ps_name)
{
    std::vector<std::string> result;

    wchar_t windowsDir[MAX_PATH];
    if (GetWindowsDirectoryW(windowsDir, MAX_PATH) == 0)
    {
        return result;
    }

    std::wstring prefetchDir = std::wstring(windowsDir) + L"\\Prefetch\\";

    std::string upperProcName = ToUpper(ps_name);
    std::wstring searchPattern = prefetchDir +
                                 std::wstring(upperProcName.begin(), upperProcName.end()) +
                                 L"-*.pf";

    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPattern.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return result;
    }

    do
    {
        // Исключаем системные ссылки на текущую и родительскую директорию
        if (wcscmp(findData.cFileName, L".") != 0 &&
            wcscmp(findData.cFileName, L"..") != 0)
        {

            std::wstring fullPath = prefetchDir + findData.cFileName;
            result.push_back(std::string(fullPath.begin(), fullPath.end()));
        }
    } while (FindNextFileW(hFind, &findData) != 0);

    FindClose(hFind);

    return result;
}

std::string FormatTimestamp(time_t timestamp)
{
    if (timestamp == 0)
        return "N/A";

    struct tm timeinfo;
    localtime_s(&timeinfo, &timestamp);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return std::string(buffer);
}

/// @brief проверка пути на "подозрительность"
bool IsSuspiciousPath(const std::wstring &path)
{
    std::wstring lower_path = path;
    std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(), ::towlower);

    // Список паттернов, которые часто встречаются в малвари
    const std::vector<std::wstring> suspicious_patterns = {
        L"\\temp\\", L"\\tmp\\", L"\\appdata\\local\\temp\\",
        L"\\users\\public\\", L"\\programdata\\",
        L"\\downloads\\", L"\\desktop\\",
        L"\\.exe", L"\\.dll", L"\\.bat", L"\\.ps1", L"\\.vbs"};

    for (const auto &pattern : suspicious_patterns)
    {
        if (lower_path.find(pattern) != std::wstring::npos)
        {
            return true;
        }
    }
    return false;
}
/// @brief Преобразование wstring к string
std::string WStringToString(const std::wstring &wstr)
{
    if (wstr.empty())
        return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), result.data(), size_needed, nullptr, nullptr);
    return result;
}