#pragma once

#include "../commands_base.hpp"

#include "storage/storage.hpp"

#include <map>
#include <memory>

#define FILE_OPTION "file"
#define PROCESS_OPTION "pid"

class AnalyseCommand : public BaseCommand {
    public:
    
    AnalyseCommand();
    ~AnalyseCommand();
    private:
    void GetFileList(); //from DB
    void AnalyzeFile(const std::string file_name);


    std::map<std::string, std::string> files_; // key = hash -> value = name
    std::unique_ptr<Storage> storage_;

};