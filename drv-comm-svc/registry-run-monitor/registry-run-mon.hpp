#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <filesystem>
#include <memory>
#include <algorithm>

namespace fs = std::filesystem;

class RegistryRunMonitor {
public:
    // Конструктор: принимает путь к файлу для сохранения снапшота (опционально)
    explicit RegistryRunMonitor(const std::optional<fs::path>& snapshotPath = std::nullopt);
    
    // Основные методы
    bool CheckAndUpdate();  // Сканирует реестр, обновляет данные, возвращает есть ли изменения
    std::vector<std::string> GetNewEntries() const;  // Возвращает имена новых записей
    std::optional<std::string> GetLatestExecutable() const;  // Путь к последнему добавленному .exe
    
    /// @brief Сохраняет текущее состояние в файл
    bool SaveSnapshot() const;
    /// @brief Загружает предыдущее состояние из файла
    bool LoadSnapshot();
    /// @brief Очищает память о предыдущем состоянии
    void ClearSnapshot();
    
    /// @brief Конфигурация: добавить какие ключи мониторить
    void AddKeyToMonitor(HKEY rootKey, const std::wstring& subKey, bool wow64_32bit = false);
    /// @brief Конфигурация: стереть данные о том, какие ключи мониторить
    void ClearMonitoredKeys();
    
private:

    struct RegistryValue {
        std::wstring valueName;
        std::wstring commandLine;
        std::string executablePath;
        DWORD lastModified;
    };
    
    struct MonitoredKey {
        HKEY rootKey;
        std::wstring subKey;
        REGSAM accessFlags;
    };
    
    std::vector<RegistryValue> ReadKeyValues(const MonitoredKey& key);
    /// @brief Извлечение пути к .exe из командной строки
    std::string ParseExecutableFromCmd(const std::wstring& commandLine);
    /// @brief Сгенерировать идентификатор ключа, который нужно мониторить
    std::string GenerateKeyIdentifier(const MonitoredKey& key, const std::wstring& valueName) const;
    /// @brief Сравнить снапшоты ключа и определить, есть ли новые значения
    bool CompareAndDetectNew(const std::unordered_map<std::string, RegistryValue>& current);
    
    std::vector<MonitoredKey> monitoredKeys_;
    std::unordered_map<std::string, RegistryValue> previousSnapshot_;
    std::unordered_map<std::string, RegistryValue> currentSnapshot_;
    std::unordered_set<std::string> newEntries_;
    std::optional<fs::path> snapshotPath_;

    size_t scanCounter_ = 0;
};