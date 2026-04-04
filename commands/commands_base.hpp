#pragma once
#include <string>
#include <vector>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class BaseCommand {
public:
    virtual ~BaseCommand() = default;
    
    /// @brief Добавление опций, специфичных для команды
    virtual void AddOptions(po::options_description& desc) = 0;
    
    /// @brief Выполнение команды после парсинга
    virtual int Execute(const po::variables_map& vm) = 0;
    
    /// @brief Получить название команды (monitor, analyse)
    virtual std::string GetName() const = 0;
    
    /// @brief Краткое описание для help
    virtual std::string GetDescription() const = 0;
};