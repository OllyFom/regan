#pragma once

#include <SQLiteCpp/Statement.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include <iostream>
#include <filesystem>
#include <boost/format.hpp>

class Storage {
    public:
    
    Storage();
    
    ~Storage();

    void StorageWrite(std::string name);

    std::map<std::string, std::string> ReadAll();
    void ClearAll();

    private:

    const std::string file_table = "files";

    const std::filesystem::path dir_name_ = "C:/Program Files/regan";
    const std::string storage_path_ = dir_name_.string() + "/storage.db";

    std::unique_ptr<SQLite::Database>   db_;  
};