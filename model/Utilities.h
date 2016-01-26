#pragma once
#include <vector>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Utilities {
    inline std::vector<std::string> GetFilesInDirectory(const std::string& directory, const std::string& extension) {
#ifdef _WIN32
        std::vector<std::string> out;
        HANDLE dir;
        WIN32_FIND_DATA file_data;

        if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
            return out;

        do {
            const std::string file_name = file_data.cFileName;
            if (file_name.find(extension) == std::string::npos) continue;

            const std::string full_file_name = directory + "/" + file_name;
            const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

            if (file_name[0] == '.')
                continue;

            if (is_directory)
                continue;

            out.push_back(file_name);
        } while (FindNextFile(dir, &file_data));

        FindClose(dir);
        return out;
#elif
        std::cerr << "GetFilesInDirectory not implemented for other operating systems than Windows"
            return out;
#endif
    }

    inline bool FileExists(const std::string& name) {
        std::ifstream f(name.c_str());
        if (f.good()) {
            f.close();
            return true;
        }
        else {
            f.close();
            return false;
        }
    }

    inline std::string GetExePath() {
        char result[MAX_PATH];
        return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
    }

    inline void ClearConsole() {
#ifdef _WIN32
        system("cls");
#elif
        std::cerr << "ClearConsole not implemented for other operating systems than Windows"
#endif
    }

    inline void ClearInputStream() {
        std::cin.clear();
        std::cin.ignore();
    }
}