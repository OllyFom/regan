// RegistryRunMonitor.cpp
#include "registry-run-mon.hpp"

#include <sstream>
#include <fstream>
#include <codecvt>
#include <locale>

/// @brief Конвертация wstring -> string (UTF-8)
static std::string WStringToUTF8(const std::wstring& wstr);
/// @brief Конвертация string -> wstring
static std::wstring UTF8ToWString(const std::string& str);


std::string RegistryRunMonitor::ParseExecutableFromCmd(const std::wstring& commandLine) {
    if (commandLine.empty()) return "";
    
    std::wstring cmd = commandLine;
    
    if (cmd.front() == L'"') {
        size_t endQuote = cmd.find(L'"', 1);
        if (endQuote != std::wstring::npos) {
            std::wstring exePath = cmd.substr(1, endQuote - 1);
            return WStringToUTF8(exePath);
        }
    }
    
    size_t spacePos = cmd.find(L' ');
    std::wstring exePath = (spacePos != std::wstring::npos) ? cmd.substr(0, spacePos) : cmd;
    
    return WStringToUTF8(exePath);
}

std::string RegistryRunMonitor::GenerateKeyIdentifier(const MonitoredKey& key, const std::wstring& valueName) const {
    std::ostringstream oss;
    oss << reinterpret_cast<uintptr_t>(key.rootKey) << "_"  
        << WStringToUTF8(key.subKey) << "_" 
        << WStringToUTF8(valueName);
    return oss.str();
}

RegistryRunMonitor::RegistryRunMonitor(const std::optional<fs::path>& snapshotPath)
    : snapshotPath_(snapshotPath) {
    
    // По умолчанию мониторим основные ключи Run
    AddKeyToMonitor(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    AddKeyToMonitor(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    // 32-битные приложения на 64-битной Windows
    AddKeyToMonitor(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", true);
    
    if (snapshotPath_ && fs::exists(*snapshotPath_)) {
        LoadSnapshot();
    }
}

void RegistryRunMonitor::AddKeyToMonitor(HKEY rootKey, const std::wstring& subKey, bool wow64_32bit) {
    MonitoredKey mk;
    mk.rootKey = rootKey;
    mk.subKey = subKey;
    mk.accessFlags = KEY_READ | (wow64_32bit ? KEY_WOW64_32KEY : KEY_WOW64_64KEY);
    monitoredKeys_.push_back(mk);
}

void RegistryRunMonitor::ClearMonitoredKeys() {
    monitoredKeys_.clear();
}

std::vector<RegistryRunMonitor::RegistryValue> 
RegistryRunMonitor::ReadKeyValues(const MonitoredKey& key) {
    
    std::vector<RegistryValue> result;
    HKEY hKey = nullptr;
    
    LONG res = RegOpenKeyExW(key.rootKey, key.subKey.c_str(), 0, key.accessFlags, &hKey);
    if (res != ERROR_SUCCESS || hKey == nullptr) {
        return result;  // Ключ не найден или нет доступа
    }
    
    DWORD index = 0;
    wchar_t valueName[256];
    DWORD valueNameSize = _countof(valueName);
    BYTE valueData[4096];
    DWORD valueDataSize = sizeof(valueData);
    DWORD valueType = 0;
    
    while (RegEnumValueW(hKey, index, valueName, &valueNameSize, nullptr, 
                         &valueType, valueData, &valueDataSize) == ERROR_SUCCESS) {
        
        if (valueType == REG_SZ || valueType == REG_EXPAND_SZ) {
            RegistryValue rv;
            rv.valueName = valueName;
            rv.commandLine = std::wstring(reinterpret_cast<wchar_t*>(valueData));
            rv.executablePath = ParseExecutableFromCmd(rv.commandLine);
            rv.lastModified = scanCounter_;  // Условный таймстемп
            result.push_back(rv);
        }
        
        valueNameSize = _countof(valueName);
        valueDataSize = sizeof(valueData);
        ++index;
    }
    
    RegCloseKey(hKey);
    return result;
}

bool RegistryRunMonitor::CheckAndUpdate() {
    ++scanCounter_;
    newEntries_.clear();
    currentSnapshot_.clear();
    
    for (const auto& key : monitoredKeys_) {
        auto values = ReadKeyValues(key);
        for (auto& val : values) {
            std::string keyId = GenerateKeyIdentifier(key, val.valueName);
            currentSnapshot_[keyId] = std::move(val);
        }
    }
    
    return CompareAndDetectNew(currentSnapshot_);
}

bool RegistryRunMonitor::CompareAndDetectNew(const std::unordered_map<std::string, RegistryValue>& current) {
    bool hasNew = false;
    
    for (const auto& [keyId, value] : current) {
        // Если записи не было в предыдущем снапшоте — это новая запись
        if (previousSnapshot_.find(keyId) == previousSnapshot_.end()) {
            newEntries_.insert(keyId);
            hasNew = true;
        }
        // TODO: Опционально: можно отслеживать изменения существующих записей
        // else if (previousSnapshot_[keyId].commandLine != value.commandLine) { ... }
    }
    
    return hasNew;
}

std::vector<std::string> RegistryRunMonitor::GetNewEntries() const {
    std::vector<std::string> result;
    result.reserve(newEntries_.size());
    
    for (const auto& keyId : newEntries_) {
        auto it = currentSnapshot_.find(keyId);
        if (it != currentSnapshot_.end()) {
            result.push_back(WStringToUTF8(it->second.valueName) + " -> " + it->second.executablePath);
        }
    }
    
    return result;
}

std::optional<std::string> RegistryRunMonitor::GetLatestExecutable() const {
    if (newEntries_.empty()) {
        return std::nullopt;
    }
    
    // Находим запись с наибольшим lastModified среди новых
    std::optional<std::string> latest;
    DWORD maxTimestamp = 0;
    
    for (const auto& keyId : newEntries_) {
        auto it = currentSnapshot_.find(keyId);
        if (it != currentSnapshot_.end() && it->second.lastModified > maxTimestamp) {
            maxTimestamp = it->second.lastModified;
            latest = it->second.executablePath;
        }
    }
    
    return latest;
}

bool RegistryRunMonitor::SaveSnapshot() const {
    if (!snapshotPath_) return false;
    
    std::ofstream file(*snapshotPath_, std::ios::binary);
    if (!file) return false;
    
    uint32_t count = static_cast<uint32_t>(currentSnapshot_.size());
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    
    for (const auto& [keyId, value] : currentSnapshot_) {

        uint32_t keyIdLen = static_cast<uint32_t>(keyId.size());
        file.write(reinterpret_cast<const char*>(&keyIdLen), sizeof(keyIdLen));
        file.write(keyId.data(), keyIdLen);

        uint32_t vNameLen = static_cast<uint32_t>(value.valueName.size());
        file.write(reinterpret_cast<const char*>(&vNameLen), sizeof(vNameLen));
        file.write(reinterpret_cast<const char*>(value.valueName.data()), vNameLen * sizeof(wchar_t));
        
        uint32_t cmdLen = static_cast<uint32_t>(value.commandLine.size());
        file.write(reinterpret_cast<const char*>(&cmdLen), sizeof(cmdLen));
        file.write(reinterpret_cast<const char*>(value.commandLine.data()), cmdLen * sizeof(wchar_t));

        uint32_t exeLen = static_cast<uint32_t>(value.executablePath.size());
        file.write(reinterpret_cast<const char*>(&exeLen), sizeof(exeLen));
        file.write(value.executablePath.data(), exeLen);
        
        file.write(reinterpret_cast<const char*>(&value.lastModified), sizeof(value.lastModified));
    }
    
    return file.good();
}

bool RegistryRunMonitor::LoadSnapshot() {
    if (!snapshotPath_ || !fs::exists(*snapshotPath_)) return false;
    
    std::ifstream file(*snapshotPath_, std::ios::binary);
    if (!file) return false;
    
    previousSnapshot_.clear();
    
    uint32_t count = 0;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    // Защита от повреждённых данных
    if (!file || count > 10000) return false;
    
    for (uint32_t i = 0; i < count; ++i) {
        RegistryValue value;
        std::string keyId;
        
        uint32_t keyIdLen = 0;
        file.read(reinterpret_cast<char*>(&keyIdLen), sizeof(keyIdLen));
        if (!file) return false;
        keyId.resize(keyIdLen);
        file.read(keyId.data(), keyIdLen);
        
        uint32_t vNameLen = 0;
        file.read(reinterpret_cast<char*>(&vNameLen), sizeof(vNameLen));
        if (!file) return false;
        value.valueName.resize(vNameLen);
        file.read(reinterpret_cast<char*>(value.valueName.data()), vNameLen * sizeof(wchar_t));
        
        uint32_t cmdLen = 0;
        file.read(reinterpret_cast<char*>(&cmdLen), sizeof(cmdLen));
        if (!file) return false;
        value.commandLine.resize(cmdLen);
        file.read(reinterpret_cast<char*>(value.commandLine.data()), cmdLen * sizeof(wchar_t));
        
        uint32_t exeLen = 0;
        file.read(reinterpret_cast<char*>(&exeLen), sizeof(exeLen));
        if (!file) return false;
        value.executablePath.resize(exeLen);
        file.read(value.executablePath.data(), exeLen);
        
        file.read(reinterpret_cast<char*>(&value.lastModified), sizeof(value.lastModified));
        if (!file) return false;
        
        previousSnapshot_[keyId] = std::move(value);
    }
    
    return true;
}

void RegistryRunMonitor::ClearSnapshot() {
    previousSnapshot_.clear();
    newEntries_.clear();
}

static std::string WStringToUTF8(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), result.data(), size, nullptr, nullptr);
    return result;
}

static std::wstring UTF8ToWString(const std::string& str) {
    if (str.empty()) return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), size);
    return result;
}