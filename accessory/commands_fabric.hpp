#pragma once
#include <iostream>
#include <memory>
#include <optional>

#include "../commands/cmds/analyser.hpp"
#include "../commands/cmds/monitor.hpp"

using std::string;
using std::optional;

class CommandsFabric {
    public: 

    CommandsFabric();
    
    static std::unique_ptr<BaseCommand> CreateCommand(const optional<string> command_name); 

    CommandsFabric(const CommandsFabric&) = delete;
    CommandsFabric(CommandsFabric&&) = delete;
    CommandsFabric& operator=(const CommandsFabric&) = delete;
    CommandsFabric& operator=(CommandsFabric&&) = delete;
    ~CommandsFabric();
   
};