#pragma once
#include "commands_base.hpp"
#include <iostream>
#include <vector>

class AnalyseCommand : public BaseCommand
{
public:
    std::string GetName() const override;
    std::string GetDescription() const override;

    void AddOptions(po::options_description &desc) override;

    int Execute(const po::variables_map &vm) override;

    AnalyseCommand();
    ~AnalyseCommand();

private:
    void GetFileList();

    void ShowHelp();

    int CmdGetList();

    int CmdAll();

    int CmdByName(const std::string &ps_name);

    int CmdByPid(const std::vector<int> &pids);

    std::string AnalysePsFiles(const std::string &ps_name);
};