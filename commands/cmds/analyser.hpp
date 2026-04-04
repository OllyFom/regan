#pragma once
#include "commands_base.hpp"
#include <iostream>
#include <vector>

// Здесь подключите ваш класс анализа Prefetch/Registry
// #include "../core/AnalyseCommand.h" (ваш существующий класс)

class AnalyseCommand : public BaseCommand {
public:
    std::string GetName() const override { return "analyse"; }
    std::string GetDescription() const override { return "Analyse bootstart execution files"; }
    
    void AddOptions(po::options_description& desc) override {
        desc.add_options()
            ("get-list", "Get new bootstart execution files list")
            ("all", "Get report about all bootstart execution files")
            ("name", po::value<std::string>(), "Get report by process name (e.g. --name \"notepad.exe\")")
            ("pid", po::value<std::vector<int>>()->multitoken(), "Analyse files for processes by PID list")
            ("help,h", "Show help for this command");
    }
    
    int Execute(const po::variables_map& vm) override {
        if (vm.count("help")) {
            ShowHelp();
            return 0;
        }
        
        if (vm.count("get-list")) {
            return CmdGetList();
        }
        if (vm.count("all")) {
            return CmdAll();
        }
        if (vm.count("name")) {
            return CmdByName(vm["name"].as<std::string>());
        }
        if (vm.count("pid")) {
            return CmdByPid(vm["pid"].as<std::vector<int>>());
        }
        
        std::cerr << "[!] No command specified. Use --help for usage.\n";
        return 1;
    }
    
private:
    void ShowHelp() {
        std::cout << "Usage: regan analyse [OPTIONS]\n\n";
        std::cout << "Options:\n";
        std::cout << "  --get-list          Get new bootstart execution files list\n";
        std::cout << "  --all               Get report about all bootstart execution files\n";
        std::cout << "  --name <process>    Get report by process name\n";
        std::cout << "  --pid <pids...>     Analyse files for processes by PID list\n";
        std::cout << "  -h, --help          Show this help\n";
    }
    
    int CmdGetList() {
        std::cout << "[*] Getting new bootstart execution files...\n";
        // TODO: Вызов RegistryRunMonitor::GetNewEntries()
        std::cout << "[✓] List retrieved\n";
        return 0;
    }
    
    int CmdAll() {
        std::cout << "[*] Generating full report...\n";
        // TODO: Генерация полного отчёта по всем записям
        std::cout << "[✓] Report generated\n";
        return 0;
    }
    
    int CmdByName(const std::string& processName) {
        std::cout << "[*] Analysing process: " << processName << "\n";
        // TODO: Вызов вашего существующего AnalysePsFiles(processName)
        std::cout << "[✓] Analysis complete\n";
        return 0;
    }
    
    int CmdByPid(const std::vector<int>& pids) {
        std::cout << "[*] Analysing " << pids.size() << " process(es) by PID...\n";
        for (int pid : pids) {
            std::cout << "  - PID: " << pid << "\n";
            // TODO: Получение имени процесса по PID и вызов анализа
        }
        std::cout << "[✓] Analysis complete\n";
        return 0;
    }
};
// #pragma once

// #include "../commands_base.hpp"

// #include "storage.hpp"

// #include <map>
// #include <memory>

// #define FILE_OPTION "file"
// #define PROCESS_OPTION "pid"

// class AnalyseCommand : public BaseCommand {
//     public:
    
//     AnalyseCommand();
//     ~AnalyseCommand();

//     private:
//     void GetFileList(); //from DB
//     void AnalyzeFile(const std::string file_name);


//     std::map<std::string, std::string> files_; // key = hash -> value = name
//     std::unique_ptr<Storage> storage_;

//     // Some output object (for pattern Bridge)

// };