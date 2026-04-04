#include "analyser.hpp"

#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::string> GetPrefetchFileNamesByPsName(const std::string &ps_name);

AnalyseCommand::AnalyseCommand()
{
    // std::cout << "AnalyseCommand constructor" << std::endl;
}

void AnalyseCommand::GetFileList()
{
    Storage storage;
    // Fill the map
}

void AnalyseCommand::AnalyzeFile(const std::string file_name)
{
}

AnalyseCommand::~AnalyseCommand()
{
    // std::cout << "AnalyseCommand destructor" << std::endl;
}

// Функция для перевода строки в верхний регистр
std::string ToUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::vector<std::string> GetPrefetchFileNamesByPsName(const std::string &ps_name)
{
    std::vector<std::string> result;

    // 1. Получаем путь к папке Windows
    wchar_t windowsDir[MAX_PATH];
    if (GetWindowsDirectoryW(windowsDir, MAX_PATH) == 0)
    {
        return result; // Пустой вектор
    }

    // 2. Формируем путь к папке Prefetch
    std::wstring prefetchDir = std::wstring(windowsDir) + L"\\Prefetch\\";

    // 3. Формируем маску поиска: ИМЯ-*.pf
    std::string upperProcName = ToUpper(processName);
    std::wstring searchPattern = prefetchDir +
                                 std::wstring(upperProcName.begin(), upperProcName.end()) +
                                 L"-*.pf";

    // 4. Начинаем поиск
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPattern.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        // Файлы не найдены или нет доступа
        return result;
    }

    // 5. Цикл сбора всех совпадений
    do
    {
        // Исключаем системные ссылки на текущую и родительскую директорию
        if (wcscmp(findData.cFileName, L".") != 0 &&
            wcscmp(findData.cFileName, L"..") != 0)
        {

            std::wstring fullPath = prefetchDir + findData.cFileName;
            result.push_back(std::string(fullPath.begin(), fullPath.end()));
        }
    } while (FindNextFileW(hFind, &findData) != 0);

    // 6. Завершаем поиск
    FindClose(hFind);

    return result;
}