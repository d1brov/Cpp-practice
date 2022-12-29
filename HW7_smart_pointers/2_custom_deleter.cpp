// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <memory>
#include <functional>

///////////////////////////////////////////////////////////////////////////////
// Implement custom deleter(which closes socket)                             //
// for std::unique_ptr in 3 different ways.                                  //
///////////////////////////////////////////////////////////////////////////////

// 1. Anonymous function
void anonimous_function_demo() {
    auto close_sock = [](SOCKET* socket) {
        if (closesocket(*socket) == SOCKET_ERROR) {
            printf("closesocket failed with error = %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        delete socket;
        printf("socket closed via anonimous function callback\n");
    };
    
    auto sock = new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if ((*sock) == INVALID_SOCKET) {
        printf("socket function failed with error = %d\n", WSAGetLastError());
        return;
    }
    
    std::unique_ptr<SOCKET, decltype(close_sock)> psock(sock, close_sock);
    printf("socket created\n");
}


// 2. Pointer to the function
void close_sock(SOCKET* socket) {
    if (closesocket(*socket) == SOCKET_ERROR) {
        printf("closesocket failed with error = %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }
    delete socket;
    printf("socket closed via pointed function callback\n");
}

void pointer_to_function_demo() {
    auto sock = new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if ((*sock) == INVALID_SOCKET) {
        printf("socket function failed with error = %d\n", WSAGetLastError());
        return;
    }

    std::unique_ptr<SOCKET, void(*)(SOCKET*)> psock(sock, close_sock);
    printf("socket created\n");
}


// 3. Functor
class Functor {
public:
    void operator()(SOCKET* socket) {
        if (closesocket(*socket) == SOCKET_ERROR) {
            printf("closesocket failed with error = %d\n", WSAGetLastError());
            WSACleanup();
            return;
        }
        delete socket;
        printf("socket closed via functor\n");
    }
};

void functor_demo() {
    auto sock = new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if ((*sock) == INVALID_SOCKET) {
        printf("socket function failed with error = %d\n", WSAGetLastError());
        return;
    }

    std::unique_ptr<SOCKET, Functor> psock(sock);
    printf("socket created\n");
}


// 4. std::function (not required)
void std_function_demo() {
    
    std::function<void(SOCKET*)> close_socket = [](SOCKET* socket) {
        if (closesocket(*socket) == SOCKET_ERROR) {
            printf("closesocket failed with error = %d\n", WSAGetLastError());
            WSACleanup();
            return;
        }
        delete socket;
        printf("socket closed via std::function\n");
    };

    auto sock = new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if ((*sock) == INVALID_SOCKET) {
        printf("socket function failed with error = %d\n", WSAGetLastError());
        return;
    }

    std::unique_ptr<SOCKET, decltype(close_socket)> p(sock, close_socket);
    printf("socket created\n");
}


int main()
{
    // Initialize Winsock
    WSADATA wsaData = { 0 };
    auto iRsult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iRsult != 0) {
        printf("WSAStartup failed: %d\n", iRsult);
        return 1;
    }

    anonimous_function_demo();

    pointer_to_function_demo();

    functor_demo();

    std_function_demo();

    return 0;
}