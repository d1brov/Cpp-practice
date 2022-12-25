#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

namespace file_util
{
    void create_file(const std::filesystem::path& path)
    {
        std::cout << "Creating " << path.string();
        std::ofstream output_file(path);
        if (!output_file)
        {
            throw std::runtime_error("Unable to create file " + path.string());
        }

        output_file.close();
        std::cout << "...DONE\n";
    }

    void write_file(const std::filesystem::path& path, const std::vector<char>& bytes)
    {
        std::cout << "Writing to " << path.string();

        // opening file
        std::ofstream output_file(path, std::ios::binary);
        if (!output_file)
        {
            throw std::runtime_error("Unable to open: " + path.string());
        }

        // writing bytes to file
        output_file.write(&bytes[0], bytes.size());

        output_file.close();

        std::cout << "...DONE\n";
    }

    std::vector<char> read_file(const std::filesystem::path& path)
    {
        std::cout << "Reading " << path.string();

        // opening file
        std::ifstream input_file(path, std::ios_base::binary);
        if (!input_file)
        {
            throw std::runtime_error("Unable to open: " + path.string());
        }

        // measuring file size
        input_file.seekg(0, std::ios_base::end);
        size_t length = input_file.tellg();
        input_file.seekg(0, std::ios_base::beg);

        // reading file to buffer
        std::vector<char> buffer;
        if (length > 0)
        {
            buffer.resize(length);
            input_file.read(&buffer[0], length);
        }

        input_file.close();

        std::cout << "...DONE\n";
        return buffer;
    }
}