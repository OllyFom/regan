#include "analyser.hpp"

AnalyseCommand::AnalyseCommand()
{
    std::cout << "AnalyseCommand constructor" << std::endl;
}

void AnalyseCommand::GetFileList() {
    Storage storage;
    // Fill the map
}

void AnalyseCommand::AnalyzeFile(const std::string file_name) {

}

AnalyseCommand::~AnalyseCommand() {
    std::cout << "AnalyseCommand destructor" << std::endl;
}