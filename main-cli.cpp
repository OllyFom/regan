#include <iostream>
#include <optional>
#include <exception>

#include "accessory/commands_fabric.hpp"
#include "accessory/parse_args.hpp"
#include "commands_base.hpp"
#include "windows.h"

#include <boost/program_options.hpp>

#include <string>

namespace po = boost::program_options;

void ShowGeneralHelp(const std::vector<std::string>& commands) {
    std::cout << "Regan Security Tool v1.0\n\n";
    std::cout << "Usage: regan <command> [OPTIONS]\n\n";
    std::cout << "Available commands:\n";
    for (const auto& cmd : commands) {
        std::cout << "  " << cmd << "\n";
    }
    std::cout << "\nUse 'regan <command> --help' for more information.\n";
}

int main(int argc, char* argv[]) {

    SetConsoleOutputCP(1251);
    
    try {

        po::options_description general_desc("General options");
        general_desc.add_options()
            ("version,v", "Show version")
            ("command", po::value<std::string>()->implicit_value(""), "Command name");
        
        po::positional_options_description pos_desc;
        pos_desc.add("command", 1);
        
        po::variables_map vm;
        auto parsed = po::command_line_parser(argc, argv)
            .options(general_desc)
            .positional(pos_desc)
            .allow_unregistered()
            .run();
        
        po::store(parsed, vm);
        po::notify(vm);
        
        if (vm.count("version")) {
            std::cout << "Regan v1.0.0\n";
            return 0;
        }
        
        if (!vm.count("command") || vm["command"].as<std::string>().empty()) {
            auto commands = CommandFactory::Instance().GetCommandNames();
            ShowGeneralHelp(commands);
            return 0;
        }

        std::string commandName = vm["command"].as<std::string>();
        auto command = CommandFactory::Instance().Create(commandName);
        
        if (!command) {
            std::cerr << "[!] Unknown command: " << commandName << "\n";
            std::cerr << "Use 'regan --help' to see available commands.\n";
            return 1;
        }
        
        // Собираем опции конкретной команды (включая --help)
        po::options_description cmd_desc(command->GetName() + " options");
        command->AddOptions(cmd_desc);
        
        // Добавляем общие опции для полного парсинга
        po::options_description full_desc("Regan Security Tool");
        full_desc.add(cmd_desc);
        
        po::variables_map cmd_vm;
        po::store(po::parse_command_line(argc, argv, full_desc), cmd_vm);
        po::notify(cmd_vm);
        
        if (cmd_vm.count("help")) {
            // Команда сама отвечает за свой help
            return command->Execute(cmd_vm);
        }
        
        return command->Execute(cmd_vm);
        
    } catch (const po::error& e) {
        std::cerr << "[!] Error: " << e.what() << "\n";
        std::cerr << "Use 'regan --help' for usage.\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[!] Fatal error: " << e.what() << "\n";
        return 1;
    }
}
