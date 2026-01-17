#include "storage.hpp"

Storage::Storage()
{
    if (!std::filesystem::is_directory(dir_name_)) {
        std::filesystem::create_directory(dir_name_);
    }
    db_ = std::make_unique<SQLite::Database>(storage_path_, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    SQLite::Transaction transaction(*db_);

    db_->exec((boost::format("CREATE TABLE IF NOT EXISTS %1% (id INTEGER PRIMARY KEY, hash TEXT, file_path TEXT);") % file_table).str());

    transaction.commit();
}

Storage::~Storage()
{
}

std::map<std::string, std::string> Storage::ReadAll()
{
    std::map<std::string, std::string> value_map;
    SQLite::Statement query(*db_, (boost::format("SELECT hash, file_path FROM %1%") % file_table).str());
    while (query.executeStep())
    {
        value_map[query.getColumn(0).getString()] = query.getColumn(1).getString();
    }
    return value_map;
}

void Storage::ClearAll()
{
    SQLite::Transaction transaction(*db_);

    SQLite::Statement query(*db_, (boost::format("SELECT id FROM %1%") % file_table).str());

    while (query.executeStep())
    {
        db_->exec((boost::format("DELETE FROM %1% WHERE id = %2%;") % file_table % query.getColumn(0)).str());
        std::cout << "Delete from line: " << query.getColumn(0) << std::endl;
    }
    transaction.commit();
}

void Storage::StorageWrite(std::string input_str)
{
    SQLite::Transaction transaction(*db_);

    db_->exec((boost::format("INSERT INTO %1% (name) VALUES ('%2%');") % file_table % input_str).str());

    transaction.commit();
}