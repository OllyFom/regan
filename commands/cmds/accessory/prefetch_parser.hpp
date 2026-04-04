#pragma once

#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <exception>
#include <memory>
#include <cstring>
#include <algorithm>

#include <Windows.h>

using namespace std;
namespace fs = std::filesystem;

fs::path GetPrefetchFileName(string required_file);

class PrefetchInfo
{
public:
    PrefetchInfo(fs::path parse_file)
        : prefetch_file_(parse_file), prog_info_{}
    {
        vector<char> file_data;

        if (!file_readed(parse_file, file_data) || file_data.size() < 0x100)
        {
            cout << "Failed to read file: " << parse_file << endl;
            throw std::runtime_error("Failed to read file and create PrefetchInfo object");
        }

        if (file_data.at(0) == 'M' && file_data.at(1) == 'A' && file_data.at(2) == 'M')
        {
            using RtlDecompressBufferEx = NTSTATUS(__stdcall *)(
                USHORT CompressionFormat,
                PUCHAR UncompressedBuffer,
                ULONG UncompressedBufferSize,
                PUCHAR CompressedBuffer,
                ULONG CompressedBufferSize,
                PULONG FinalUncompressedSize,
                PVOID WorkSpace);
            using RtlGetCompressionWorkSpaceSize = NTSTATUS(__stdcall *)(
                USHORT CompressionFormatAndEngine,
                PULONG CompressBufferWorkSpaceSize,
                PULONG CompressFragmentWorkSpaceSize);

            static auto compression_workspace_size = reinterpret_cast<RtlGetCompressionWorkSpaceSize>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetCompressionWorkSpaceSize"));
            static auto decompress_buffer_ex = reinterpret_cast<RtlDecompressBufferEx>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlDecompressBufferEx"));

            if (file_data.size() < 8)
                return;

            const auto signature = *reinterpret_cast<std::uint32_t *>(file_data.data());
            const auto decompressed_size = *reinterpret_cast<std::uint32_t *>(file_data.data() + 0x4);
            if ((signature & 0x00FFFFFF) != 0x004d414d)
                return;

            file_data.erase(file_data.begin(), file_data.begin() + 8);

            if ((signature & 0xF0000000) >> 28)
            {
                return;
            }

            const auto compression_format = (signature & 0x0F000000) >> 24;

            ULONG compressed_buffer_workspace_size, compress_fragment_workspace_size;
            if (compression_workspace_size(compression_format, &compressed_buffer_workspace_size, &compress_fragment_workspace_size) != 0)
                return;

            std::vector<char> decompressed_data(decompressed_size);

            ULONG final_uncompressed_size;

            auto *const workspace = malloc(compressed_buffer_workspace_size);
            if (!workspace)
                return;

            if (decompress_buffer_ex(
                    compression_format,
                    reinterpret_cast<PUCHAR>(decompressed_data.data()),
                    decompressed_size,
                    reinterpret_cast<PUCHAR>(file_data.data()),
                    file_data.size(),
                    &final_uncompressed_size,
                    workspace) != 0)
            {
                free(workspace);
                return;
            }

            free(workspace);

            prefetch_file_data_ = move(decompressed_data);
        }
        else if (file_data.at(4) == 'S' && file_data.at(5) == 'C' && file_data.at(6) == 'C' && file_data.at(7) == 'A')
            prefetch_file_data_ = move(file_data);

        get_filenames_strings();
        get_run_count();
        get_exec_file();
        get_exec_time();
    }

    time_t GetExecTime() const
    {
        return prog_info_.exec_time_;
    }
    fs::path GetExecFileName() const
    {
        return prog_info_.exec_file_name_;
    }
    int GetRunCount() const
    {
        return prog_info_.run_count_;
    }
    vector<wstring> GetUsedFiles() const
    {
        return prog_info_.used_files_;
    }

    std::string GetFullInfo() const
    {
        if (!success())
            return "Failed to parse prefetch file";

        std::stringstream info;

        info << "=== Prefetch File Information ===\n";
        info << "Executable: " << wstring_to_string(prog_info_.exec_file_name_) << "\n";
        info << "File size: " << file_size() << " bytes\n";
        info << "Run count: " << prog_info_.run_count_ << "\n";
        info << "Last run: " << prog_info_.exec_time_ << "\n";
        info << "Files referenced: " << prog_info_.used_files_.size() << "\n";

        return info.str();
    }

private:
    void get_run_count()
    {
        prog_info_.run_count_ = run_count();
    }

    void get_exec_time()
    {
        prog_info_.exec_time_ = executed_time();
    }

    void get_exec_file()
    {
        if (!success())
            return;

        constexpr size_t exe_name_offset = 0x10;

        if (exe_name_offset >= prefetch_file_data_.size())
            return;

        wstring result;
        const wchar_t *name_ptr = reinterpret_cast<const wchar_t *>(prefetch_file_data_.data() + exe_name_offset);

        for (size_t i = 0;; i += sizeof(wchar_t))
        {
            if (exe_name_offset + i + sizeof(wchar_t) > prefetch_file_data_.size())
                break;

            wchar_t ch;
            memcpy(&ch, prefetch_file_data_.data() + exe_name_offset + i, sizeof(wchar_t));

            if (ch == L'\0')
                break;

            result.push_back(ch);
        }
        prog_info_.exec_file_name_ = move(result);
    }

    bool file_readed(const fs::path &name, vector<char> &out)
    {
        ifstream file(name, ios::binary);
        if (!file.good())
            return false;

        file.unsetf(ios::skipws);

        file.seekg(0, ios::end);
        const size_t size = file.tellg();
        file.seekg(0, ios::beg);

        out.resize(size);

        file.read(out.data(), size);

        file.close();

        return true;
    }

#define SETUP_VARIABLE(type, name, offset)                                      \
    [[nodiscard]] type name() const                                             \
    {                                                                           \
        type var;                                                               \
        std::memcpy(&var, prefetch_file_data_.data() + (offset), sizeof(type)); \
        return var;                                                             \
    }

    SETUP_VARIABLE(int, version, 0x0)
    SETUP_VARIABLE(int, signature, 0x4)
    SETUP_VARIABLE(int, file_size, 0xC)
    SETUP_VARIABLE(int, file_name_strings_offset, 0x64)
    SETUP_VARIABLE(int, file_name_strings_size, 0x68)
    SETUP_VARIABLE(int, volume_information_offset, 0x6C)
    SETUP_VARIABLE(int, volumes_count, 0x70)
    SETUP_VARIABLE(int, volumes_information_size, 0x74)
    SETUP_VARIABLE(int, run_count, 0xd0)
    SETUP_VARIABLE(uintptr_t, executed_timestamp, 0x80)

    bool success() const
    {
        return !prefetch_file_data_.empty();
    }

    void get_filenames_strings()
    {
        // TODO: normal VOLUME name like C:, D:, and other
        const auto offset = this->file_name_strings_offset();
        const auto size = this->file_name_strings_size();

        const auto begin = prefetch_file_data_.begin() + offset;
        const auto end = begin + size;
        vector<char> filenames(begin, end);

        vector<wstring> resources;
        wstring name;

        for (size_t i = 0; i < filenames.size(); i += sizeof(wchar_t))
        {
            const auto ch = *reinterpret_cast<const wchar_t *>(&filenames[i]);
            if (ch == L'\0')
            {
                if (!name.empty())
                {
                    resources.push_back(name);
                    name.clear();
                }
                continue;
            }
            name.push_back(ch);
        }

        if (!name.empty())
        {
            resources.push_back(name);
        }

        prog_info_.used_files_ = move(resources);
    }

    time_t executed_time() const
    {
        const auto filetime_to_timet = [](const FILETIME &ft)
        {
            ULARGE_INTEGER ull;
            ull.LowPart = ft.dwLowDateTime;
            ull.HighPart = ft.dwHighDateTime;

            return ull.QuadPart / 10000000ULL - 11644473600ULL;
        };

        ULARGE_INTEGER file_time;
        file_time.QuadPart = executed_timestamp();

        const auto file_time_ptr = reinterpret_cast<const FILETIME *>(&file_time);
        return filetime_to_timet(*file_time_ptr);
    }

    std::string wstring_to_string(const std::wstring &wstr) const
    {
        if (wstr.empty())
            return "";

        std::string result;
        result.reserve(wstr.size());

        for (wchar_t wc : wstr)
        {
            if (wc <= 0x7F)
            {
                result.push_back(static_cast<char>(wc));
            }
            else
            {
                result.push_back('?');
            }
        }

        return result;
    }

private:
    struct prog_info
    {
        vector<wstring> used_files_;
        fs::path exec_file_name_;
        size_t run_count_;
        time_t exec_time_;
    };

    prog_info prog_info_;
    fs::path prefetch_file_;
    vector<char> prefetch_file_data_;
};
