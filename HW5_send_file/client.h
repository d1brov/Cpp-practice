#pragma once

#include <ostream>
#include <string>
#include <exception>
#include <boost/asio.hpp>
#include <boost/iostreams/filtering_stream.hpp>
//#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <filesystem>

#include "file_pds.h"

class client
{
private:
    boost::asio::ip::tcp::endpoint endpoint_;

public:
    client(const std::string& host, const std::string& port) :
        endpoint_(boost::asio::ip::address::from_string(host), std::stoi(port))
    {
    }

    void send_file(const std::filesystem::path& path)
    {
        //read file to serializable object
        file_pds file
        {
            path.stem().string(),
            path.extension().string(),
            file_util::read_file(path)
        };

        start_streaming(file);
    }

private:
    void start_streaming(const file_pds& file)
    {
        boost::asio::ip::tcp::iostream socket_stream(endpoint_);
        boost::archive::binary_oarchive bin_oarchive(socket_stream);

        // serialize & stream object to server
        std::cout << "Streaming file";

        try
        {
            bin_oarchive << file;
        }
        catch (std::exception& e)
        {
            std::cerr << "Cannot serialize file to stream. Reason: " << e.what() <<'\n';
        }
        
        socket_stream.close();
        std::cout << "...DONE\n";
    }
};