Client-Server async echo socket app for Linux platform.
Allows to send messages from multiple clients to the server and receive an echo 
from the server (Except if the message "hello", then the response from the 
server will be "world").

Asyncronity is implemented via Nonblocking I/O with select().
https://www.ibm.com/docs/en/i/7.5?topic=designs-example-nonblocking-io-select

---- USAGE ----
Server:
    client <port> <max_clients>
    port - port that server runs on;
    max_clients - the number of simultaneous connections.

Client:
    server <host> <port>
    host - ipv4 address of the server to connect;
    port - port of the server to connect.


---- BUILDING ----
The app consists of two independent sources client.cpp and server.cpp
that compiles independently.

gcc [client.cpp|server.cpp] -o [client|server]