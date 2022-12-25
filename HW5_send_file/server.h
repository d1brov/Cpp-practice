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
#include <optional>
#include <boost/archive/binary_iarchive.hpp>

#include "file_pds.h"
#include "file_utils.h"

//class session : public std::enable_shared_from_this<session>
//{
//public:
//
//    session(boost::asio::ip::tcp::socket&& socket) : 
//        socket(std::move(socket))
//    {
//        std::cout << "Session created\n";
//    }
//
//    void start()
//    {
//        std::cout << "Session has started\n";
//    }
//
//private:
//
//    boost::asio::ip::tcp::socket socket;
//    boost::asio::streambuf streambuf;
//};

class server
{
private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::iostream socket_stream_;

public:
    server(boost::asio::io_context& io_context, const std::string& port) :
        io_context_(io_context),
        acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), stoi(port)))
    {
        std::cout << "Server has started at 127.0.0.1:" << port << '\n';
        accept_stream_connect();
    }

    ~server()
    {
        acceptor_.close();
        socket_stream_.close();
        std::cout << "Server has finished\n";
    }

private:

    void handle_stream()
    {
        file_pds file_pds;

        try
        {
            boost::archive::binary_iarchive bin_iarchive(socket_stream_);
            bin_iarchive >> file_pds;
        }
        catch (std::exception& e)
        {
            std::cerr << "Cannot deserialize file from stream. Reason: " << e.what() << '\n';
            return;
        }

        std::cout << "Received file: " << file_pds.name << file_pds.extension << '\n';

        try
        {
            file_util::create_file("received_" + file_pds.name + file_pds.extension);
            file_util::write_file("received_" + file_pds.name + file_pds.extension, file_pds.bytes);
        }
        catch (std::exception& e)
        {
            std::cerr << "Cannot create file. Reason: " << e.what() << '\n';
            return;
        }
    }

    void accept_stream_connect()
    {
        boost::system::error_code ec;
        while(true)
        {
            std::cout << "Waiting for incoming stream...\n";
            acceptor_.accept(*socket_stream_.rdbuf(), ec);
            std::cout << "Start receiving incoming stream\n";
            handle_stream();
            socket_stream_.close();
            std::cout << "Done receiving incoming stream\n\n";
        }
    }
};