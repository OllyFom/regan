#include "commands_fabric.hpp"

CommandsFabric::CommandsFabric() {
    std::cout << "CommandsFabric constructor" << std::endl;
}
CommandsFabric::~CommandsFabric() {
    std::cout << "CommandsFabric destructor" << std::endl;
}

std::unique_ptr<BaseCommand> CommandsFabric::CreateCommand(const optional<string> comm_name){
    if (!comm_name.has_value()) {
        std::cout << "Args has no command name" << std::endl;
        return nullptr;
    }
    if (comm_name.value() == "analyse") {
        
        std::cout << "Analyser" << std::endl;
        return std::make_unique<AnalyseCommand>();

    }
    else if (comm_name.value() == "monitor") {

        std::cout << "Monitor" << std::endl;
        return std::make_unique<MonitorCommand>();

    }
    else {

        std::cout << "Failed to found command by name";
        // some exception
        return nullptr;

    }
}
