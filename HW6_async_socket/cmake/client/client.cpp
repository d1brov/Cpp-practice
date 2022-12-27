#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUF_SIZE 256

void start_client(const unsigned short port)
{
    int return_code;

    // Initialize socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket initialization success\n";

    // Connect to socket
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    return_code = connect(socket_fd, (struct sockaddr *)&addr, addrlen);
    if (return_code < 0)
    {
        perror("connect() failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected\n";

    // Messaging loop Client <--> Server
    char rx_message[MAX_BUF_SIZE] = {0};
    char tx_message[MAX_BUF_SIZE] = {0};
    for (;;)
    {
        // Send message to server
        std::cout << "Enter message for server: ";
        fgets(tx_message, MAX_BUF_SIZE, stdin);
        return_code = send(socket_fd, tx_message, strlen(tx_message), 0);
        if (return_code < 0)
        {
            perror("send() failed");
            exit(EXIT_FAILURE);
        }

        // Receive message from server untill EWOULDBLOCK
        return_code = recv(socket_fd, rx_message, MAX_BUF_SIZE, 0);

        if (return_code == 0)
        {
            std::cout << "Connection closed\n";
            close(socket_fd);
            break;
        }

        if (return_code < 0)
        {
            perror("recv() failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Server responded: " << rx_message;
    }
}

int main(int argc, char **argv)
{
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>";
        return EXIT_FAILURE;
    }

    try
    {
        start_client(std::stoi(argv[1]));
    }
    catch(std::invalid_argument const& e)
    {
        std::cerr << "Invalid argument" << '\n';
        return EXIT_FAILURE;
    }

    return 0;
}