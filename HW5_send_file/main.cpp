#include <chrono>

#include "server.h"
#include "client.h"

void start_server(const std::string& port)
{
    boost::asio::io_context io_context;
    server server(io_context, port);
    io_context.run();
}

void start_client(const std::string& host, const std::string& port, const std::string& path)
{
    try {
        client client(host, port);
        client.send_file(path);
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[])
{
    //start_server("15001");

    if (argc < 2)
    {
        std::cerr << "Specify mode: server/client\n";
        return 1;
    }

    if (strcmp(argv[1], "server") == 0)
    {
        if (argc < 3)
        {
            std::cerr << "Usage: " << argv[0] << "server <port>\n";
            return 1;
        }
        start_server(argv[2]);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        if (argc < 5)
        {
            std::cerr << "Usage: " << argv[0] << "client <server_host> <server_port> <file_path>\n";
            return 1;
        }
        start_client(argv[2], argv[3], argv[4]);
    }

    //start_server("15003");
    //start_client("127.0.0.1", "15003");

    //std::thread srv_t(start_server, "15003");
    //std::this_thread::sleep_for(std::chrono::milliseconds(300));
    //std::thread clt_t(start_client, "127.0.0.1", "15003");

    return 0;
}