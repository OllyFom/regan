
#pragma once

#include <iostream>

#include "commands_base.hpp"

class MonitorCommand : public BaseCommand {
public:
    std::string GetName() const override;
    std::string GetDescription() const override;
    
    void AddOptions(po::options_description& desc) override;
    
    int Execute(const po::variables_map& vm);
    
private:
    void ShowHelp();
    
    int CmdStatus();
    
    int CmdStart();
    
    int CmdStop();
    
    int CmdEnableBoot();
    
    int CmdDisableBoot();
};