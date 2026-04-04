#pragma once
#include "commands_base.hpp"
#include "cmds/monitor.hpp"
#include "cmds/analyser.hpp"
#include <memory>
#include <unordered_map>
#include <functional>

class CommandFactory {
public:
    using CommandCreator = std::function<std::unique_ptr<BaseCommand>()>;
    
    static CommandFactory& Instance() {
        static CommandFactory instance;
        return instance;
    }
    
    void Register(const std::string& name, CommandCreator creator) {
        creators_[name] = std::move(creator);
    }
    
    std::unique_ptr<BaseCommand> Create(const std::string& name) {
        auto it = creators_.find(name);
        if (it == creators_.end()) {
            return nullptr;
        }
        return it->second();
    }
    
    std::vector<std::string> GetCommandNames() const {
        std::vector<std::string> names;
        for (const auto& [name, _] : creators_) {
            names.push_back(name);
        }
        return names;
    }
    
private:
    CommandFactory() {
        Register("monitor", []() { return std::make_unique<MonitorCommand>(); });
        Register("analyse", []() { return std::make_unique<AnalyseCommand>(); });
    }
    
    std::unordered_map<std::string, CommandCreator> creators_;
};