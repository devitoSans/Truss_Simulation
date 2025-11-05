#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <iterator>

inline std::string read_file_lines(const std::string& path)
{
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Failed to open file: " + path);
    std::string lines, line;
    while (std::getline(in, line)) lines.append(line + "\n");
    return lines;
}


#endif